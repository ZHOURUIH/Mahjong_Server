﻿#ifndef _NET_SERVER_H_
#define _NET_SERVER_H_

#include "PacketDefine.h"
#include "ServerDefine.h"
#include "ThreadLock.h"
#include "ServerBase.h"
#include "CustomThread.h"
#include "txCommandReceiver.h"

class Packet;
class NetClient;
class PacketFactoryManager;
class NetServer : public ServerBase, public txCommandReceiver
{
public:
	NetServer();
	virtual ~NetServer(){ destroy(); }
	void destroy();
	virtual void init(const int& port, const int& backLog);
	virtual void update(const float& elapsedTime);

	static const float& getHeartBeatTimeOut() { return mHeartBeatTimeOut; }
	static const bool& getOutputLog(){ return mOutputLog; }
	const int& getPort() { return mPort; }
	int getClientCount() { return mClientList.size(); }
	
	virtual CLIENT_GUID notifyAcceptClient(const TX_SOCKET& socket, const char* ip);
	void sendMessage(Packet* packet, const CLIENT_GUID& clientGUID, const bool& destroyPacketEndSend = true);
	void sendMessage(Packet* packet, NetClient* client, const bool& destroyPacketEndSend = true);
	static Packet* createPacket(const PACKET_TYPE& type);
	static void destroyPacket(Packet* packet);
	virtual void disconnectSocket(const CLIENT_GUID& client);	// 涓庡鎴风鏂紑杩炴帴,鍙兘鍦ㄤ富绾跨▼涓皟鐢?
	NetClient* getClient(const CLIENT_GUID& clientGUID);
protected:
	static bool acceptSocket(void* args);
	static bool receiveSendSocket(void* args);
	void processSend();
	void processRecv();
	CLIENT_GUID generateSocketGUID() { return mSocketGUIDSeed++; }
#if RUN_PLATFORM == PLATFORM_LINUX
	static void signalProcess(int signalNum);
#endif
protected:
	int mPort;                 // 绔彛鍙?
	unsigned int mMaxSocket;
	TX_SOCKET mSocket;
	CustomThread* mAcceptThread;
	CustomThread* mReceiveThread;
	ThreadLock mClientLock;
	txMap<CLIENT_GUID, NetClient*> mClientList;	// 瀹㈡埛绔垪琛?
	static CLIENT_GUID mSocketGUIDSeed;
	static PacketFactoryManager* mPacketFactoryManager;
	static float mHeartBeatTimeOut;
	static bool mOutputLog;
};

#endif
