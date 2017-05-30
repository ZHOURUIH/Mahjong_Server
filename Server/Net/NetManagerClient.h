#ifndef _NET_MANAGER_CLIENT_H_
#define _NET_MANAGER_CLIENT_H_

#include "ServerDefine.h"
#include "ServerBase.h"
#include "txThreadLock.h"

class Packet;
class NetManagerClient : public ServerBase
{
public:
	NetManagerClient(const CLIENT_GUID& clientGUID, const TX_SOCKET& s, const char* ip);
	virtual ~NetManagerClient(){ destroy(); }
	void update(const float& elapsedTime);
	void destroy();
	// autoDestroyPacket表示是否自动销毁该消息包
	void sendPacket(Packet* packet, const bool& autoDestroyPacket);
	void notifyReceiveHeartBeat()						{ mHeartBeatTime = 0.0f; }
	void notifyPlayerLogin(const CHAR_GUID& charGUID)	{ mCharGUID = charGUID; }
	void notifyRecvEmpty()								{ mIsDeadClient = true; }
	void notifyDataSended(const int& sendedCount);
	void notifyRecvData(const char* data, const int& dataCount);

	// 获得成员变量
	const CLIENT_GUID& getClientGUID()	{ return mClientGUID; }
	const CHAR_GUID& getCharGUID()		{ return mCharGUID; }
	const TX_SOCKET& getSocket()		{ return mSocket; }
	const char* getIP()					{ return mIP; }
	const bool& isDeadClient()			{ return mIsDeadClient; }
	const int& getSendDataCount()		{ return mSendDataCount; }
	const char* getSendData()			{ return mSendBuffer; }
	const int& getRecvDataCount()		{ return mRecvDataCount; }
	const char* getRecvData()			{ return mRecvBuffer; }
protected:
	TX_SOCKET mSocket;			// 客户端套接字
	CLIENT_GUID mClientGUID;	// 客户端唯一ID
	CHAR_GUID mCharGUID;		// 客户端的角色唯一ID
	char* mTempBuffer1;			// 临时缓冲区,用于临时存放单个消息包数据,不含包头
	char* mTempBuffer0;			// 临时缓冲区,用于临时存放单个消息包数据,含包头
	char* mRecvBuffer;			// 客户端接收数据缓冲区
	char* mSendBuffer;			// 客户端发送数据缓冲区
	int mSendDataCount;			// 客户端发送缓冲区中已经有的数据长度
	int mRecvDataCount;			// 客户端接收缓冲区中未解析的数据长度
	char mIP[16];				// 客户端的IP地址
	float mHeartBeatTime;		// 客户端上一次的心跳到当前的时间,秒
	float mConnectTime;			// 客户端连接到服务器的时间,秒
	bool mIsDeadClient;			// 该客户端是否已经断开连接或者心跳超时
	txThreadLock mSendLock;		// 发送的线程锁
	txThreadLock mRecvLock;		// 接收的线程锁
};

#endif