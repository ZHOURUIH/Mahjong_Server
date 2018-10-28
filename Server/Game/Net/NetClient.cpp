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
mCharacterGUID(INVALID_ID),
mAccountGUID(INVALID_ID),
mTempBuffer1(NULL),
mTempBuffer0(NULL),
mHeartBeatTime(0.0f),
mConnectTime(0.0f),
mIsDeadClient(false),
mAsyncExecute(false)
{
	mWriteExecuteIndex = 0;
	mReadExecuteIndex = 1;
	TRACE_NEW(StreamBuffer, mSendStreamBuffer, CLIENT_SEND_BUFFER_SIZE);
	TRACE_NEW(StreamBuffer, mRecvStreamBuffer, CLIENT_RECV_BUFFER_SIZE);
	TRACE_NEW(StreamBuffer, mTempRecvStreamBuffer, CLIENT_RECV_BUFFER_SIZE);
	memset(mIP, 0, 16);
	memcpy(mIP, ip, MathUtility::getMax(16, (int)strlen(ip)));
	TRACE_NEW_ARRAY(char, CLIENT_TEMP_BUFFER_SIZE, mTempBuffer0);
	TRACE_NEW_ARRAY(char, CLIENT_TEMP_BUFFER_SIZE, mTempBuffer1);
}

void NetClient::destroy()
{
	// 如果角色已经登录了,则通知角色管理器玩家已经断开连接
	if (mCharacterGUID != INVALID_ID)
	{
		CommandCharacterManagerNotifyPlayerOffline* cmdOffline = NEW_CMD_DELAY_INFO(cmdOffline);
		cmdOffline->mPlayerID = mCharacterGUID;
		pushDelayCommand(cmdOffline, mCharacterManager);
	}
	TRACE_DELETE_ARRAY(mTempBuffer0);
	TRACE_DELETE_ARRAY(mTempBuffer1);
	TRACE_DELETE(mSendStreamBuffer);
	TRACE_DELETE(mRecvStreamBuffer);
	TRACE_DELETE(mTempRecvStreamBuffer);
	// 关闭客户端套接字,并从列表移除
	CLOSE_SOCKET(mSocket);
}

void NetClient::update(float elapsedTime)
{
	if (mIsDeadClient)
	{
		return;
	}
	// 同步执行消息时,在主线程更新中执行所有消息
	if (!mAsyncExecute)
	{
		executeAllPacket();
	}
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
	int packetSize = packet->getSize();
	packet->write(mTempBuffer1, packetSize);
	// 写入类型,数据长度,消息内容
	int offset = 0;
	BinaryUtility::write(mTempBuffer0, CLIENT_TEMP_BUFFER_SIZE, offset, (int)packet->getPacketType());
	BinaryUtility::write(mTempBuffer0, CLIENT_TEMP_BUFFER_SIZE, offset, packetSize);
	BinaryUtility::writeBuffer(mTempBuffer0, CLIENT_TEMP_BUFFER_SIZE, offset, mTempBuffer1, packetSize);
	if (NetServer::getOutputLog())
	{
		clientInfo("已发送 : " + packet->debugInfo());
	}

	// 将数据拷贝到客户端主缓冲区中
	LOCK(mSendLock);
	mSendStreamBuffer->addDataToInputBuffer(mTempBuffer0, HEADER_SIZE + packetSize);
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
	mSendStreamBuffer->removeDataFromInputBuffer(sendedCount);
	UNLOCK(mSendLock);
}

void NetClient::notifyRecvData(const char* data, int dataCount)
{
	LOCK(mTempRecvLock);
	mTempRecvStreamBuffer->addDataToInputBuffer(data, dataCount);
	UNLOCK(mTempRecvLock);
}

void NetClient::parseData()
{
	// 同步缓冲区
	LOCK(mTempRecvLock);
	mRecvStreamBuffer->addDataToInputBuffer(mTempRecvStreamBuffer->getData(), mTempRecvStreamBuffer->getDataLength());
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
		if (parseResult == PR_SUCCESS)
		{
			packetList.push_back(packet);
			// 将已经解析的数据移除
			mRecvStreamBuffer->removeDataFromInputBuffer(index);
		}
		// 解析出现错误
		else if (parseResult == PR_ERROR)
		{
			clientInfo("wrong packet data!");
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
	// 将解析后的消息列表同步到列表中
	int packetCount = packetList.size();
	for(int i = 0; i < packetCount; ++i)
	{
		mExecutePacketList[mWriteExecuteIndex].push_back(packetList[i]);
	}

	// 异步执行消息时,在解析线程就执行所有消息
	if (mAsyncExecute)
	{
		executeAllPacket();
	}
}

PARSE_RESULT NetClient::parsePacket(int& index, Packet*& packet)
{
	// 可能还没有接收完全,等待下次接收
	if (mRecvStreamBuffer->getDataLength() < index + HEADER_SIZE)
	{
		return PR_NOT_ENOUGH;
	}
	PACKET_TYPE type = (PACKET_TYPE)BinaryUtility::read<int>(mRecvStreamBuffer->getData(), mRecvStreamBuffer->getBufferSize(), index);
	int packetSize = BinaryUtility::read<int>(mRecvStreamBuffer->getData(), mRecvStreamBuffer->getBufferSize(), index);
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
	if (mRecvStreamBuffer->getDataLength() >= index + packetSize)
	{
		// 执行该消息包
		packet = NetServer::createPacket(type);
		if (!packet->read(mRecvStreamBuffer->getData() + index, packetSize))
		{
			// 解析错误
			NetServer::destroyPacket(packet);
			packet = NULL;
			return PR_ERROR;
		}
		packet->mClientID = mClientGUID;
		packet->mClient = this;
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

bool NetClient::sendPacketCheck(Packet* packet)
{
	// 检查客户端是否还连接
	if (mIsDeadClient)
	{
		return false;
	}
	// 不需要设置消息包中的客户端ID
	if (packet->mClientID != INVALID_ID)
	{
		clientError("packet's client ID should be none!");
		return false;
	}
	// 保存发送数据
	int packetSize = packet->getSize();
	int sendSize = HEADER_SIZE + packetSize;
	// 判断临时缓冲区大小是否足够
	if (sendSize > CLIENT_TEMP_BUFFER_SIZE)
	{
		clientError("临时缓冲区太小! send size : " + StringUtility::intToString(sendSize) + ", temp buffer size : " + StringUtility::intToString(CLIENT_TEMP_BUFFER_SIZE));
		return false;
	}
	// 判断缓冲区是否还能存放下当前要发送的消息数据
	if (!mSendStreamBuffer->isAvailable(packetSize))
	{
		// 缓冲区已堆满,客户端网络阻塞严重,断开客户端
		clientError("客户端缓冲区已满!将断开与客户端的链接!");
		mIsDeadClient = true;
		return false;
	}
	return true;
}

void NetClient::executeAllPacket()
{
	// 首先交换缓冲区
	LOCK(mExecuteIndexLock);
	MathUtility::swap(mReadExecuteIndex, mWriteExecuteIndex);
	UNLOCK(mExecuteIndexLock);
	// 执行读缓冲区中的所有消息包
	txVector<Packet*>& executeList = mExecutePacketList[mReadExecuteIndex];
	int count = executeList.size();
	FOR(executeList, int i = 0; i < count; ++i)
	{
		Packet* packetReply = executeList[i];
		if (NetServer::getOutputLog())
		{
			clientInfo("execute : type : " + StringUtility::intToString(packetReply->getPacketType()) + ", desc : " + packetReply->debugInfo());
		}
		packetReply->execute();
		NetServer::destroyPacket(packetReply);
	}
	END(executeList);
	executeList.clear();
}

void NetClient::clientInfo(const std::string& info)
{
	LOG_INFO("IP : " + std::string(mIP) + ", 账号GUID : " + StringUtility::intToString(mAccountGUID) + ", 角色GUID : " + StringUtility::intToString(mCharacterGUID) + " || " + info);
}

void NetClient::clientError(const std::string& info)
{
	LOG_ERROR("IP : " + std::string(mIP)  + ", 账号GUID : " + StringUtility::intToString(mAccountGUID) + ", 角色GUID : " + StringUtility::intToString(mCharacterGUID) + " || " + info);
}