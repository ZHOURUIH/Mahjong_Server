#ifndef _NET_MANAGER_SERVER_H_
#define _NET_MANAGER_SERVER_H_

#include "PacketDefine.h"
#include "ServerDefine.h"
#include "txThreadLock.h"
#include "ServerBase.h"
#include "txCommandReceiver.h"

class Packet;
class NetManagerClient;
class PacketFactoryManager;
class NetManagerServer : public ServerBase, public txCommandReceiver
{
public:
	NetManagerServer();
	virtual ~NetManagerServer(){ destroy(); }
	void destroy();
	virtual void init(int port, int backLog);
	virtual void update(const float& elapsedTime);

	static const float& getHeartBeatTimeOut() { return mHeartBeatTimeOut; }
	static const bool& getOutputLog(){ return mOutputLog; }
	const int& getPort() { return mPort; }
	int getClientCount() { return mClientList.size(); }
	
	virtual CLIENT_GUID notifyAcceptClient(const SOCKET& socket, const char* ip);
	void sendMessage(Packet* packet, const CLIENT_GUID& clientGUID, const bool& destroyPacketEndSend = true);
	void sendMessage(Packet* packet, NetManagerClient* client, const bool& destroyPacketEndSend = true);
	static Packet* createPacket(const PACKET_TYPE& type);
	static void destroyPacket(Packet* packet);
	virtual void disconnectSocket(const CLIENT_GUID& client);	// 与客户端断开连接,只能在主线程中调用
	NetManagerClient* getClient(const CLIENT_GUID& clientGUID)
	{
		NetManagerClient* client = NULL;
		LOCK(mClientLock, LT_READ);
		txMap<CLIENT_GUID, NetManagerClient*>::iterator iterClient = mClientList.find(clientGUID);
		if (iterClient != mClientList.end())
		{
			client = iterClient->second;
		}
		UNLOCK(mClientLock, LT_READ);
		return client;
	}
protected:
#if RUN_PLATFORM == PLATFORM_WINDOWS
	static DWORD WINAPI acceptSocket(LPVOID lpParameter);
	static DWORD WINAPI receiveSendSocket(LPVOID lpParameter);
#elif RUN_PLATFORM == PLATFORM_LINUX
	static void* acceptSocket(void* args);
	static void* receiveSendSocket(void* args);
#endif
	void processSend();
	void processRecv();
	CLIENT_GUID generateSocketGUID() { return mSocketGUIDSeed++; }
#if RUN_PLATFORM == PLATFORM_LINUX
	static void signalProcess(int signalNum);
#endif
protected:
	int mPort;                 // 端口号
	int mBackLog;
	unsigned int mMaxSocket;
	TX_SOCKET mSocket;
	TX_THREAD mAcceptThread;
	TX_THREAD mReceiveThread;
	txThreadLock mClientLock;
	txMap<CLIENT_GUID, NetManagerClient*> mClientList;	// 客户端列表
	static CLIENT_GUID mSocketGUIDSeed;
	static PacketFactoryManager* mPacketFactoryManager;
	static float mHeartBeatTimeOut;
	static bool mOutputLog;
};

#endif
