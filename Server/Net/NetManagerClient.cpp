#include "NetManagerClient.h"
#include "txMemeryTrace.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CommandHeader.h"
#include "CharacterManager.h"
#include "PacketFactoryManager.h"

NetManagerClient::NetManagerClient(const CLIENT_GUID& clientGUID, const TX_SOCKET& s, const char* ip)
:
mSocket(s),
mClientGUID(clientGUID),
mCharGUID(~0),
mTempBuffer1(NULL),
mTempBuffer0(NULL),
mRecvBuffer(NULL),
mSendBuffer(NULL),
mSendDataCount(0),
mRecvDataCount(0),
mHeartBeatTime(0.0f),
mConnectTime(0.0f),
mIsDeadClient(false)
{
	memset(mIP, 0, 16);
	memcpy(mIP, ip, TX_MAX(16, strlen(ip)));
	mSendBuffer = TRACE_NEW_ARRAY(char, CLIENT_BUFFER_SIZE, mSendBuffer);
	mRecvBuffer = TRACE_NEW_ARRAY(char, CLIENT_BUFFER_SIZE, mRecvBuffer);
	mTempBuffer0 = TRACE_NEW_ARRAY(char, CLIENT_TEMP_BUFFER_SIZE, mTempBuffer0);
	mTempBuffer1 = TRACE_NEW_ARRAY(char, CLIENT_TEMP_BUFFER_SIZE, mTempBuffer1);
}

void NetManagerClient::destroy()
{
	// 如果角色已经登录了,则通知角色管理器玩家已经断开连接
	if (mCharGUID != (CHAR_GUID)~0)
	{
		CommandCharacterManagerNotifyPlayerOffline* cmdOffline = txCommand::createDelayCommand<CommandCharacterManagerNotifyPlayerOffline>(COMMAND_PARAM);
		cmdOffline->mPlayerID = mCharGUID;
		mCommandSystem->pushDelayCommand(cmdOffline, mCharacterManager);
	}

	TRACE_DELETE_ARRAY(mSendBuffer);
	TRACE_DELETE_ARRAY(mRecvBuffer);
	TRACE_DELETE_ARRAY(mTempBuffer0);
	TRACE_DELETE_ARRAY(mTempBuffer1);

	// 关闭客户端套接字,并从列表移除
#if RUN_PLATFORM == PLATFORM_WINDOWS
	closesocket(mSocket);
#elif RUN_PLATFORM == PLATFORM_LINUX
	close(mSocket);
#endif
}

void NetManagerClient::update(const float& elapsedTime)
{
	std::vector<Packet*> packetList;
	LOCK(mRecvLock, LT_WRITE);
	// 解析接收到的数据
	int index = 0;
	bool ret = true;
	while (true)
	{
		// 可能还没有接收完全,等待下次接收
		if (index + HEADER_SIZE > mRecvDataCount)
		{
			break;
		}
		PACKET_TYPE type = (PACKET_TYPE)(*(short*)(mRecvBuffer + index));
		index += sizeof(short);
		int packetSize = (int)(*(short*)(mRecvBuffer + index));
		index += sizeof(short);
		// 包长度错误
		if (packetSize < 0)
		{
			ret = false;
			break;
		}
		
		// 验证包长度是否正确
		int factorySize = PacketFactoryManager::getPacketSize(type);
		if (packetSize != factorySize)
		{
			LOG_INFO("error : wrong packet data! packet : %d, need size : %d, receive size : %d", type, factorySize, packetSize);
			ret = false;
			break;
		}

		// 如果该包已经接收完全
		if (index + packetSize <= mRecvDataCount)
		{
			// 执行该消息包
			Packet* packetReply = NetManagerServer::createPacket(type);
			packetReply->read(mRecvBuffer + index, packetSize);
			packetReply->mClient = mClientGUID;
			packetList.push_back(packetReply);
			index += packetSize;
			// 已经解析完了
			if (index == mRecvDataCount)
			{
				break;
			}
		}
		// 未接收完全,等待下次接收
		else
		{
			// 将下标重置到包头
			index -= HEADER_SIZE;
			break;
		}
	}
	// 如果消息解析失败则将该客户端断开
	if (!ret)
	{
		// 先设置为断开连接的客户端
		mIsDeadClient = true;
	}
	// 如果没有发生解析错误,则将已解析的数据清空
	else
	{
		memmove(mRecvBuffer, mRecvBuffer + index, mRecvDataCount - index);
		mRecvDataCount -= index;
	}
	UNLOCK(mRecvLock, LT_WRITE);

	int packetCount = packetList.size();
	for (int i = 0; i < packetCount; ++i)
	{
		Packet* packetReply = packetList[i];
		if (NetManagerServer::getOutputLog())
		{
			LOG_INFO("%s | execute : type : %d, desc : %s", txUtility::getTime(), (int)packetReply->getPacketType(), packetReply->debugInfo().c_str());
		}
		packetReply->execute();
		NetManagerServer::destroyPacket(packetReply);
	}

	mHeartBeatTime += elapsedTime;
	mConnectTime += elapsedTime;
	if (mHeartBeatTime >= NetManagerServer::getHeartBeatTimeOut())
	{
		mIsDeadClient = true;
	}
}

void NetManagerClient::sendPacket(Packet* packet, const bool& autoDestroyPacket)
{
	// 检查客户端是否还连接
	if (mIsDeadClient)
	{
		if (autoDestroyPacket)
		{
			NetManagerServer::destroyPacket(packet);
		}
		return;
	}
	// 不需要设置消息包中的客户端ID
	if (packet->mClient != (CLIENT_GUID)~0)
	{
		if (autoDestroyPacket)
		{
			NetManagerServer::destroyPacket(packet);
		}
		GAME_ERROR("error : packet's client ID should be none!");
		return;
	}
	// 保存发送数据
	const short& packetSize = packet->getSize();
	int sendSize = HEADER_SIZE + packetSize;
	// 判断临时缓冲区大小是否足够
	if (sendSize > CLIENT_TEMP_BUFFER_SIZE)
	{
		if (autoDestroyPacket)
		{
			NetManagerServer::destroyPacket(packet);
		}
		GAME_ERROR("temp buffer is too small! send size : %d, temp buffer size : %d", sendSize, CLIENT_TEMP_BUFFER_SIZE);
		return;
	}
	// 判断缓冲区是否还能存放下当前要发送的消息数据
	if (mSendDataCount + sendSize > CLIENT_BUFFER_SIZE)
	{
		if (autoDestroyPacket)
		{
			NetManagerServer::destroyPacket(packet);
		}
		// 缓冲区已堆满,客户端网络阻塞严重,断开客户端
		LOG_INFO("client socket buffer is full!");
		mNetManagerServer->disconnectSocket(mClientGUID);
		return;
	}
	int offset = 0;
	short shortType = (short)packet->getPacketType();
	// 写入类型(short)
	Packet::writeByte(mTempBuffer0, (char*)(&shortType), offset, sendSize, sizeof(shortType));
	// 写入数据长度(short)
	Packet::writeByte(mTempBuffer0, (char*)(&packetSize), offset, sendSize, sizeof(packetSize));
	// 写入消息内容(char*)
	packet->write(mTempBuffer1, packetSize);
	Packet::writeByte(mTempBuffer0, mTempBuffer1, offset, sendSize, packetSize);

	LOCK(mSendLock, LT_WRITE);
	// 将数据拷贝到客户端主缓冲区中
	memcpy(mSendBuffer + mSendDataCount, mTempBuffer0, sendSize);
	mSendDataCount += sendSize;
	UNLOCK(mSendLock, LT_WRITE);

	// 销毁消息包
	if (autoDestroyPacket)
	{
		NetManagerServer::destroyPacket(packet);
	}
}

void NetManagerClient::notifyDataSended(const int& sendedCount)
{
	LOCK(mSendLock, LT_WRITE);
	if (sendedCount < mSendDataCount)
	{
		// 将后面未发送的字节移动到前面,重置数据长度
		memmove(mSendBuffer, mSendBuffer + sendedCount, mSendDataCount - sendedCount);
	}
	mSendDataCount -= sendedCount;
	UNLOCK(mSendLock, LT_WRITE);
}

void NetManagerClient::notifyRecvData(const char* data, const int& dataCount)
{
	// 检查还是否能接收数据
	if (mRecvDataCount + dataCount > CLIENT_BUFFER_SIZE)
	{
		// 暂时报错
		GAME_ERROR("error : recv data is full!");
		return;
	}
	LOCK(mRecvLock, LT_WRITE);
	memcpy(mRecvBuffer + mRecvDataCount, data, dataCount);
	mRecvDataCount += dataCount;
	UNLOCK(mRecvLock, LT_WRITE);
	if (NetManagerServer::getOutputLog())
	{
		LOG_INFO("%s | recv : ip : %s, size : %d", txUtility::getTime(), mIP, dataCount);
	}
}
