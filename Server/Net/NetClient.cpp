﻿#include "NetClient.h"
#include "txMemoryTrace.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CommandHeader.h"
#include "CharacterManager.h"
#include "PacketFactoryManager.h"
#include "Utility.h"

NetClient::NetClient(const CLIENT_GUID& clientGUID, const TX_SOCKET& s, const char* ip)
:
mSocket(s),
mClientGUID(clientGUID),
mCharGUID(INVALID_ID),
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
	memcpy(mIP, ip, txMath::getMax(16, (int)strlen(ip)));
	mSendBuffer = TRACE_NEW_ARRAY(char, CLIENT_BUFFER_SIZE, mSendBuffer);
	mRecvBuffer = TRACE_NEW_ARRAY(char, CLIENT_BUFFER_SIZE, mRecvBuffer);
	mTempBuffer0 = TRACE_NEW_ARRAY(char, CLIENT_TEMP_BUFFER_SIZE, mTempBuffer0);
	mTempBuffer1 = TRACE_NEW_ARRAY(char, CLIENT_TEMP_BUFFER_SIZE, mTempBuffer1);
}

void NetClient::destroy()
{
	// 如果角色已经登录了,则通知角色管理器玩家已经断开连接
	if (mCharGUID != INVALID_ID)
	{
		CommandCharacterManagerNotifyPlayerOffline* cmdOffline = txCommand::createDelayCommand<CommandCharacterManagerNotifyPlayerOffline>(CMD_PARAM);
		cmdOffline->mPlayerID = mCharGUID;
		mCommandSystem->pushDelayCommand(cmdOffline, mCharacterManager);
	}

	TRACE_DELETE_ARRAY(mSendBuffer);
	TRACE_DELETE_ARRAY(mRecvBuffer);
	TRACE_DELETE_ARRAY(mTempBuffer0);
	TRACE_DELETE_ARRAY(mTempBuffer1);

	// 关闭客户端套接字,并从列表移除
	CLOSE_SOCKET(mSocket);
}

void NetClient::update(const float& elapsedTime)
{
	txVector<Packet*> packetList;
	LOCK(mRecvLock);
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
			Packet* packetReply = NetServer::createPacket(type);
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
	UNLOCK(mRecvLock);

	int packetCount = packetList.size();
	FOR_STL(packetList, int i = 0; i < packetCount; ++i)
	{
		Packet* packetReply = packetList[i];
		if (NetServer::getOutputLog())
		{
			LOG_INFO("%s | execute : type : %d, desc : %s", txUtility::getTime(), (int)packetReply->getPacketType(), packetReply->debugInfo().c_str());
		}
		packetReply->execute();
		NetServer::destroyPacket(packetReply);
	}
	END_FOR_STL(packetList);

	mHeartBeatTime += elapsedTime;
	mConnectTime += elapsedTime;
	if (mHeartBeatTime >= NetServer::getHeartBeatTimeOut())
	{
		mIsDeadClient = true;
	}
}

void NetClient::sendPacket(Packet* packet, const bool& autoDestroyPacket)
{
	// 检查客户端是否还连接
	if (mIsDeadClient)
	{
		if (autoDestroyPacket)
		{
			NetServer::destroyPacket(packet);
		}
		return;
	}
	// 不需要设置消息包中的客户端ID
	if (packet->mClient != INVALID_ID)
	{
		if (autoDestroyPacket)
		{
			NetServer::destroyPacket(packet);
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
			NetServer::destroyPacket(packet);
		}
		GAME_ERROR("temp buffer is too small! send size : %d, temp buffer size : %d", sendSize, CLIENT_TEMP_BUFFER_SIZE);
		return;
	}
	// 判断缓冲区是否还能存放下当前要发送的消息数据
	if (mSendDataCount + sendSize > CLIENT_BUFFER_SIZE)
	{
		if (autoDestroyPacket)
		{
			NetServer::destroyPacket(packet);
		}
		// 缓冲区已堆满,客户端网络阻塞严重,断开客户端
		LOG_INFO("client socket buffer is full!");
		mNetServer->disconnectSocket(mClientGUID);
		return;
	}
	int offset = 0;
	const short& shortType = (short)packet->getPacketType();
	// 写入类型(short)
	Packet::writeByte(mTempBuffer0, (char*)(&shortType), offset, sendSize, sizeof(shortType));
	// 写入数据长度(short)
	Packet::writeByte(mTempBuffer0, (char*)(&packetSize), offset, sendSize, sizeof(packetSize));
	// 写入消息内容(char*)
	packet->write(mTempBuffer1, packetSize);
	Packet::writeByte(mTempBuffer0, mTempBuffer1, offset, sendSize, packetSize);

	LOCK(mSendLock);
	// 将数据拷贝到客户端主缓冲区中
	memcpy(mSendBuffer + mSendDataCount, mTempBuffer0, sendSize);
	mSendDataCount += sendSize;
	UNLOCK(mSendLock);

	// 销毁消息包
	if (autoDestroyPacket)
	{
		NetServer::destroyPacket(packet);
	}
}

void NetClient::notifyDataSended(const int& sendedCount)
{
	LOCK(mSendLock);
	if (sendedCount < mSendDataCount)
	{
		// 将后面未发送的字节移动到前面,重置数据长度
		memmove(mSendBuffer, mSendBuffer + sendedCount, mSendDataCount - sendedCount);
	}
	mSendDataCount -= sendedCount;
	UNLOCK(mSendLock);
}

void NetClient::notifyRecvData(const char* data, const int& dataCount)
{
	// 检查还是否能接收数据
	if (mRecvDataCount + dataCount > CLIENT_BUFFER_SIZE)
	{
		// 暂时报错
		GAME_ERROR("error : recv data is full!");
		return;
	}
	LOCK(mRecvLock);
	memcpy(mRecvBuffer + mRecvDataCount, data, dataCount);
	mRecvDataCount += dataCount;
	UNLOCK(mRecvLock);
	if (NetServer::getOutputLog())
	{
		LOG_INFO("%s | recv : ip : %s, size : %d", txUtility::getTime(), mIP, dataCount);
	}
}
