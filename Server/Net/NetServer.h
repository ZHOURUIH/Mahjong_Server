#ifndef _NET_SERVER_H_
#define _NET_SERVER_H_

#include "PacketDefine.h"
#include "ThreadLock.h"
#include "CustomThread.h"
#include "FrameComponent.h"

class Packet;
class NetClient;
class PacketFactoryManager;
class CharacterPlayer;
class NetServer : public FrameComponent
{
public:
	NetServer(const std::string& name);
	virtual ~NetServer(){ destroy(); }
	void destroy();
	virtual void init();
	virtual void update(float elapsedTime);

	static float getHeartBeatTimeOut()	{ return mHeartBeatTimeOut; }
	static bool getOutputLog()			{ return mOutputLog; }
	int getPort()						{ return mPort; }
	int getClientCount()				{ return mClientList.size(); }
	bool isAvailable()					{ return mSocket != INVALID_SOCKET; }
	
	virtual CLIENT_GUID notifyAcceptClient(TX_SOCKET socket, const char* ip);
	void sendMessage(Packet* packet, CharacterPlayer* player, bool destroyPacketEndSend = true);
	void sendMessage(Packet* packet, CLIENT_GUID clientGUID, bool destroyPacketEndSend = true);
	void sendMessage(Packet* packet, NetClient* client, bool destroyPacketEndSend = true);
	static Packet* createPacket(PACKET_TYPE type);
	template<typename T>
	static T* createPacket(T*& packet, PACKET_TYPE type)
	{
		return static_cast<T*>(createPacket(type));
	}
	static void destroyPacket(Packet* packet);
	virtual void disconnectSocket(CLIENT_GUID client);	// 与客户端断开连接,只能在主线程中调用
	NetClient* getClient(CLIENT_GUID clientGUID);
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
	int mPort;                 // 端口号
	unsigned int mMaxSocket;
	TX_SOCKET mSocket;
	CustomThread* mAcceptThread;
	CustomThread* mReceiveThread;
	ThreadLock mClientLock;
	txMap<CLIENT_GUID, NetClient*> mClientList;	// 客户端列表
	static CLIENT_GUID mSocketGUIDSeed;
	static PacketFactoryManager* mPacketFactoryManager;
	static float mHeartBeatTimeOut;
	static bool mOutputLog;
	float mServerHeartBeatTimeout;	// 服务器自身心跳间隔时间
	float mCurServerHeartBeatTime;
	int mServerHeartBeat;			// 服务器自身的心跳,用于判断服务器是否还在继续运行
};

#endif
