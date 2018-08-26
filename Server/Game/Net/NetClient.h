#ifndef _NET_CLIENT_H_
#define _NET_CLIENT_H_

#include "ServerDefine.h"
#include "ServerBase.h"
#include "ThreadLock.h"
#include "StreamBuffer.h"

class Packet;
class NetClient : public ServerBase
{
public:
	NetClient(CLIENT_GUID clientGUID, TX_SOCKET s, const char* ip);
	virtual ~NetClient(){ destroy(); }
	void update(float elapsedTime);
	void destroy();
	// autoDestroyPacket表示是否自动销毁该消息包
	void sendPacket(Packet* packet, bool autoDestroyPacket);
	void notifyReceiveHeartBeat()				{ mHeartBeatTime = 0.0f; }
	void notifyPlayerLogin(CHAR_GUID charGUID)	{ mCharGUID = charGUID; }
	void notifyRecvEmpty()						{ mIsDeadClient = true; }
	void notifyDataSended(int sendedCount);
	void notifyRecvData(const char* data, int dataCount);

	// 获得成员变量
	CLIENT_GUID getClientGUID()	{ return mClientGUID; }
	CHAR_GUID getCharGUID()		{ return mCharGUID; }
	TX_SOCKET getSocket()		{ return mSocket; }
	const char* getIP()			{ return mIP; }
	bool isDeadClient()			{ return mIsDeadClient; }
	int getSendDataCount()		{ return mSendStreamBuffer->mDataLength; }
	const char* getSendData()	{ return mSendStreamBuffer->mBuffer; }
	int getRecvDataCount()		{ return mRecvStreamBuffer->mDataLength; }
	const char* getRecvData()	{ return mRecvStreamBuffer->mBuffer; }
protected:
	bool sendPacketCheck(Packet* packet);
	PARSE_RESULT parsePacket(int& index, Packet*& packet);
protected:
	TX_SOCKET mSocket;			// 客户端套接字
	CLIENT_GUID mClientGUID;	// 客户端唯一ID
	CHAR_GUID mCharGUID;		// 客户端的角色唯一ID
	char* mTempBuffer1;			// 临时缓冲区,用于临时存放单个消息包数据,不含包头
	char* mTempBuffer0;			// 临时缓冲区,用于临时存放单个消息包数据,含包头
	StreamBuffer* mSendStreamBuffer;// 客户端发送数据缓冲区
	StreamBuffer* mRecvStreamBuffer;// 客户端接收数据缓冲区
	StreamBuffer* mTempRecvStreamBuffer;	// 接收的临时缓冲区
	char mIP[16];				// 客户端的IP地址
	float mHeartBeatTime;		// 客户端上一次的心跳到当前的时间,秒
	float mConnectTime;			// 客户端连接到服务器的时间,秒
	bool mIsDeadClient;			// 该客户端是否已经断开连接或者心跳超时
	ThreadLock mSendLock;		// 发送的线程锁
	ThreadLock mTempRecvLock;		// 接收的线程锁
};

#endif