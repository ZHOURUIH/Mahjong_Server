#include "NetClient.h"
#include "txMemoryTrace.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CommandHeader.h"
#include "CharacterManager.h"
#include "PacketFactoryManager.h"
#include "Utility.h"

NetClient::NetClient(CLIENT_GUID clientGUID, TX_SOCKET s, const char* ip)
:
mSocket(s),
mClientGUID(clientGUID),
mCharGUID(INVALID_ID),
mTempBuffer1(NULL),
mTempBuffer0(NULL),
mHeartBeatTime(0.0f),
mConnectTime(0.0f),
mIsDeadClient(false)
{
	mSendStreamBuffer = TRACE_NEW(StreamBuffer, mSendStreamBuffer, CLIENT_BUFFER_SIZE);
	mRecvStreamBuffer = TRACE_NEW(StreamBuffer, mRecvStreamBuffer, CLIENT_BUFFER_SIZE);
	mTempRecvStreamBuffer = TRACE_NEW(StreamBuffer, mTempRecvStreamBuffer, CLIENT_BUFFER_SIZE);
	memset(mIP, 0, 16);
	memcpy(mIP, ip, MathUtility::getMax(16, (int)strlen(ip)));
	mTempBuffer0 = TRACE_NEW_ARRAY(char, CLIENT_TEMP_BUFFER_SIZE, mTempBuffer0);
	mTempBuffer1 = TRACE_NEW_ARRAY(char, CLIENT_TEMP_BUFFER_SIZE, mTempBuffer1);
}

void NetClient::destroy()
{
	// 如果角色已经登录了,则通知角色管理器玩家已经断开连接
	if (mCharGUID != INVALID_ID)
	{
		CommandCharacterManagerNotifyPlayerOffline* cmdOffline = NEW_CMD_DELAY_INFO(cmdOffline);
		cmdOffline->mPlayerID = mCharGUID;
		mCommandSystem->pushDelayCommand(cmdOffline, mCharacterManager);
	}
	TRACE_DELETE_ARRAY(mTempBuffer0);
	TRACE_DELETE_ARRAY(mTempBuffer1);
	TRACE_DELETE(mSendStreamBuffer);
	TRACE_DELETE(mRecvStreamBuffer);
	TRACE_DELETE(mTempRecvStreamBuffer);
	// 关闭客户端套接字,并从列表移除
	CLOSE_SOCKET(mSocket);
}

PARSE_RESULT NetClient::parsePacket(int& index, Packet*& packet)
{
	// 可能还没有接收完全,等待下次接收
	if (mRecvStreamBuffer->mDataLength < index + HEADER_SIZE)
	{
		return PR_NOT_ENOUGH;
	}
	PACKET_TYPE type = (PACKET_TYPE)BinaryUtility::read<short>(mRecvStreamBuffer->mBuffer, mRecvStreamBuffer->mBufferSize, index);
	int packetSize = (int)BinaryUtility::read<short>(mRecvStreamBuffer->mBuffer, mRecvStreamBuffer->mBufferSize, index);
	if (packetSize < 0)
	{
		return PR_ERROR;
	}
	// 验证包长度是否正确
	int factorySize = PacketFactoryManager::getPacketSize(type);
	if (packetSize != factorySize)
	{
		return PR_ERROR;
	}
	// 如果该包已经接收完全
	if (mRecvStreamBuffer->mDataLength >= index + packetSize)
	{
		// 执行该消息包
		packet = NetServer::createPacket(type);
		if (!packet->read(mRecvStreamBuffer->mBuffer + index, packetSize))
		{
			// 解析错误
			NetServer::destroyPacket(packet);
			packet = NULL;
			return PR_ERROR;
		}
		packet->mClient = mClientGUID;
		index += packetSize;
	}
	// 未接收完全,等待下次接收
	else
	{
		// 将下标重置到包头
		index -= HEADER_SIZE;
		return PR_NOT_ENOUGH;
	}
	return PR_SUCCESS;
}

void NetClient::update(float elapsedTime)
{
	// 同步缓冲区
	LOCK(mTempRecvLock);
	mRecvStreamBuffer->addDataToInputBuffer(mTempRecvStreamBuffer->mBuffer, mTempRecvStreamBuffer->mDataLength);
	mTempRecvStreamBuffer->clearInputBuffer();
	UNLOCK(mTempRecvLock);

	// 解析接收到的数据
	txVector<Packet*> packetList;
	bool ret = true;
	while (true)
	{
		Packet* packet = NULL;
		int index = 0;
		PARSE_RESULT parseResult = parsePacket(index, packet);
		// 数据未接收完全
		if(parseResult == PR_SUCCESS)
		{
			packetList.push_back(packet);
			// 将已经解析的数据移除
			mRecvStreamBuffer->removeDataFromInputBuffer(0, index);
		}
		// 解析出现错误
		else if (parseResult == PR_ERROR)
		{
			LOG_INFO("error : wrong packet data!");
			ret = false;
			break;
		}
		// 未接收完全,继续等待接收
		else if (parseResult == PR_NOT_ENOUGH)
		{
			break;
		}
	}
	// 如果消息解析失败则将该客户端断开
	if (!ret)
	{
		// 先设置为断开连接的客户端
		mIsDeadClient = true;
	}

	int packetCount = packetList.size();
	FOR(packetList, int i = 0; i < packetCount; ++i)
	{
		Packet* packetReply = packetList[i];
		if (NetServer::getOutputLog())
		{
			LOG_INFO("execute : type : %d, desc : %s", (int)packetReply->getPacketType(), packetReply->debugInfo().c_str());
		}
		packetReply->execute();
		NetServer::destroyPacket(packetReply);
	}
	END(packetList);

	mHeartBeatTime += elapsedTime;
	mConnectTime += elapsedTime;
	if (mHeartBeatTime >= NetServer::getHeartBeatTimeOut())
	{
		mIsDeadClient = true;
	}
}

void NetClient::sendPacket(Packet* packet, bool autoDestroyPacket)
{
	if (!sendPacketCheck(packet))
	{
		if (autoDestroyPacket)
		{
			NetServer::destroyPacket(packet);
		}
		return;
	}
	// 保存发送数据
	short packetSize = packet->getSize();
	int sendSize = HEADER_SIZE + packetSize;
	int offset = 0;
	short type = (short)packet->getPacketType();
	packet->write(mTempBuffer1, packetSize);
	// 写入类型,数据长度,消息内容
	BinaryUtility::write(mTempBuffer0, CLIENT_TEMP_BUFFER_SIZE, offset, type);
	BinaryUtility::write(mTempBuffer0, CLIENT_TEMP_BUFFER_SIZE, offset, packetSize);
	BinaryUtility::writeBuffer(mTempBuffer0, CLIENT_TEMP_BUFFER_SIZE, offset, mTempBuffer1, packetSize);

	// 将数据拷贝到客户端主缓冲区中
	LOCK(mSendLock);
	mSendStreamBuffer->addDataToInputBuffer(mTempBuffer0, sendSize);
	UNLOCK(mSendLock);

	// 销毁消息包
	if (autoDestroyPacket)
	{
		NetServer::destroyPacket(packet);
	}
}

void NetClient::notifyDataSended(int sendedCount)
{
	LOCK(mSendLock);
	mSendStreamBuffer->removeDataFromInputBuffer(0, sendedCount);
	UNLOCK(mSendLock);
}

void NetClient::notifyRecvData(const char* data, int dataCount)
{
	LOCK(mTempRecvLock);
	mTempRecvStreamBuffer->addDataToInputBuffer(data, dataCount);
	UNLOCK(mTempRecvLock);
	if (NetServer::getOutputLog())
	{
		LOG_INFO("recv : ip : %s, size : %d", mIP, dataCount);
	}
}

bool NetClient::sendPacketCheck(Packet* packet)
{
	// 检查客户端是否还连接
	if (mIsDeadClient)
	{
		return false;
	}
	// 不需要设置消息包中的客户端ID
	if (packet->mClient != INVALID_ID)
	{
		LOG_ERROR("error : packet's client ID should be none!");
		return false;
	}
	// 保存发送数据
	short packetSize = packet->getSize();
	int sendSize = HEADER_SIZE + packetSize;
	// 判断临时缓冲区大小是否足够
	if (sendSize > CLIENT_TEMP_BUFFER_SIZE)
	{
		LOG_ERROR("temp buffer is too small! send size : %d, temp buffer size : %d", sendSize, CLIENT_TEMP_BUFFER_SIZE);
		return false;
	}
	// 判断缓冲区是否还能存放下当前要发送的消息数据
	if (!mSendStreamBuffer->isAvailable(packetSize))
	{

		// 缓冲区已堆满,客户端网络阻塞严重,断开客户端
		LOG_INFO("client socket buffer is full!");
		mNetServer->disconnectSocket(mClientGUID);
		return false;
	}
	return true;
}