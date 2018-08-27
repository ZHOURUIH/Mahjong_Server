#include "NetServer.h"
#include "GameLog.h"
#include "CommandHeader.h"
#include "ServerConfig.h"
#include "PacketFactoryManager.h"
#include "NetClient.h"
#include "TimeLock.h"
#include "CharacterPlayer.h"

CLIENT_GUID NetServer::mSocketGUIDSeed = 1;
float NetServer::mHeartBeatTimeOut = 0.0f;
bool NetServer::mOutputLog = true;
PacketFactoryManager* NetServer::mPacketFactoryManager = NULL;

NetServer::NetServer(const std::string& name)
:FrameComponent(name)
{
	mReceiveThread = TRACE_NEW(CustomThread, mReceiveThread, "ReceiveSocket");
	mAcceptThread = TRACE_NEW(CustomThread, mAcceptThread, "AcceptSocket");
	mSocket = INVALID_SOCKET;
	mPort = 0;
	mMaxSocket = 0;
	mServerHeartBeatTimeout = 10.0f;
	mCurServerHeartBeatTime = 0.0f;
	mPacketFactoryManager = TRACE_NEW(PacketFactoryManager, mPacketFactoryManager);
}

void NetServer::destroy()
{
	auto iterClient = mClientList.begin();
	auto iterClientEnd = mClientList.end();
	FOR(mClientList, ; iterClient != iterClientEnd; ++iterClient)
	{
		TRACE_DELETE(iterClient->second);
	}
	END(mClientList);
	mClientList.clear();
	TRACE_DELETE(mPacketFactoryManager);
	TRACE_DELETE(mReceiveThread);
	TRACE_DELETE(mAcceptThread);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	WSACleanup();
#endif
	CLOSE_SOCKET(mSocket);
}

void NetServer::init()
{
#if RUN_PLATFORM == PLATFORM_LINUX
	signal(SIGPIPE, signalProcess);
#endif
	mPort = (int)ServerConfig::getFloatParam(SDF_SOCKET_PORT);
	mHeartBeatTimeOut = ServerConfig::getFloatParam(SDF_HEART_BEAT_TIME_OUT);
	mOutputLog = (int)ServerConfig::getFloatParam(SDF_OUTPUT_NET_LOG) > 0;
	// 初始化工厂
	mPacketFactoryManager->init();

	//初始化Socket环境
#if RUN_PLATFORM == PLATFORM_WINDOWS
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		LOG_ERROR("WSAStartup failed!");
		return;
	}
#endif
	//创建监听的Socket
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
	{
		LOG_ERROR("socket failed!");
		return;
	}

	mMaxSocket = mSocket;

	//设置服务器Socket地址
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(mPort);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
#elif RUN_PLATFORM == PLATFORM_LINUX
	addrServ.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
	//绑定Sockets Server
	if (bind(mSocket, (const struct sockaddr*)&addrServ, sizeof(SOCKADDR_IN)) != 0)
	{
		mSocket = INVALID_SOCKET;
		LOG_ERROR("bind failed!");
		return;
	}
	//在Sockets Server上进行监听
	if (listen(mSocket, (int)ServerConfig::getFloatParam(SDF_BACK_LOG)) != 0)
	{
		mSocket = INVALID_SOCKET;
		LOG_ERROR("listen failed!");
		return;
	}
	mReceiveThread->start(receiveSendSocket, this);
	mAcceptThread->start(acceptSocket, this);
}

bool NetServer::acceptSocket(void* args)
{
	NetServer* netManager = (NetServer*)(args);
	sockaddr_in addr;
#if RUN_PLATFORM == PLATFORM_WINDOWS
	int nLen = sizeof(addr);
#elif RUN_PLATFORM == PLATFORM_LINUX
	socklen_t nLen = sizeof(addr);
#endif
	char ip[32] = { 0 };
	TX_SOCKET sClient = accept(netManager->mSocket, (struct sockaddr*)&addr, &nLen);
	if (sClient == (TX_SOCKET)INVALID_SOCKET)
	{
		LOG_ERROR("error : accept failed!");
		return true;
	}
	// 获得客户端IP,然后通知已经接收到一个客户端的连接
	if (netManager->mOutputLog)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		SPRINTF(ip, 32, "%d.%d.%d.%d", addr.sin_addr.S_un.S_un_b.s_b1, addr.sin_addr.S_un.S_un_b.s_b2, addr.sin_addr.S_un.S_un_b.s_b3, addr.sin_addr.S_un.S_un_b.s_b4);
#elif RUN_PLATFORM == PLATFORM_LINUX
		int ip0 = GET_BYTE(addr.sin_addr.s_addr, 0);
		int ip1 = GET_BYTE(addr.sin_addr.s_addr, 1);
		int ip2 = GET_BYTE(addr.sin_addr.s_addr, 2);
		int ip3 = GET_BYTE(addr.sin_addr.s_addr, 3);
		SPRINTF(ip, 16, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);
#endif
	}
	CommandServerNotifyAcceptedClient* cmdAccepted = NEW_CMD_DELAY(cmdAccepted);
	cmdAccepted->mSocket = sClient;
	cmdAccepted->mIP = ip;
	mCommandSystem->pushDelayCommand(cmdAccepted, netManager);
	return true;
}

bool NetServer::receiveSendSocket(void* args)
{
	NetServer* netManager = (NetServer*)(args);
	// 有客户端连接到了服务器,才接收数据和发送数据
	if (netManager->getClientCount() > 0)
	{
		netManager->processRecv();
		// 处理发送socket消息
		netManager->processSend();
	}
	return true;
}

void NetServer::processSend()
{
	txVector<NetClient*> selectedClient;
	timeval tv = { 0, 0 };	// select查看后立即返回
	fd_set fdwrite;
	// 等待发送列表解锁,然后锁定发送列表
	LOCK(mClientLock);
	try
	{
		auto iterClient = mClientList.begin();
		auto iterClientEnd = mClientList.end();
		FOR(mClientList, ; iterClient != iterClientEnd;)
		{
			FD_ZERO(&fdwrite);
			selectedClient.clear();
			// 不能超过最大并发连接数,采用FD_SETSIZE一组的轮询方式
			for (int i = 0; iterClient != iterClientEnd && i < FD_SETSIZE; ++iterClient, ++i)
			{
				// 只检查有数据需要写的客户端
				if (iterClient->second->getSendDataCount() > 0)
				{
					selectedClient.push_back(iterClient->second);
					FD_SET(iterClient->second->getSocket(), &fdwrite);
				}
			}
			// select第一个参数在windows中可以为0,但是在其他系统中需要设置
			int selectRet = select(mMaxSocket + 1, NULL, &fdwrite, NULL, &tv);
			// 如果有客户端可写
			if (selectRet > 0)
			{
				int selectedClientCount = selectedClient.size();
				FOR(selectedClient, int i = 0; i < selectedClientCount; ++i)
				{
					if (FD_ISSET(selectedClient[i]->getSocket(), &fdwrite))
					{
						if (mOutputLog)
						{
							LOG_INFO("send to client : %s", selectedClient[i]->getIP());
						}
						int sendedCount = send(selectedClient[i]->getSocket(), selectedClient[i]->getSendData(), selectedClient[i]->getSendDataCount(), 0);
						// 检查是否send有错误
						if (sendedCount < 0)
						{
							if (errno == EPIPE)
							{
								LOG_ERROR("管道损坏错误信号，send error : EPIPE");
							}
							else if (errno == EAGAIN)
							{
								LOG_ERROR("重试错误信号，send error : EAGAIN");
							}
						}
						else
						{
							selectedClient[i]->notifyDataSended(sendedCount);
						}
					}
				}
				END(selectedClient);
			}
		}
		END(mClientList);
	}
	catch (std::exception e)
	{
		LOG_ERROR("exception : %s", e.what());
	}
	// 解锁发送列表
	UNLOCK(mClientLock);
}

void NetServer::processRecv()
{
	static char buff[CLIENT_BUFFER_SIZE];
	memset(buff, 0, CLIENT_BUFFER_SIZE);
	txVector<NetClient*> selectedClient;
	timeval tv = { 0, 0 };	// select查看后立即返回
	fd_set fdread;
	// 等待解锁accept列表的读写,并锁定accept列表
	LOCK(mClientLock);
	try
	{
		auto iterClient = mClientList.begin();
		auto iterClientEnd = mClientList.end();
		FOR(mClientList, ; iterClient != iterClientEnd;)
		{
			FD_ZERO(&fdread);
			selectedClient.clear();
			// 不能超过最大并发连接数,采用FD_SETSIZE一组的轮询方式
			for (int i = 0; iterClient != iterClientEnd && i < FD_SETSIZE; ++iterClient, ++i)
			{
				selectedClient.push_back(iterClient->second);
				FD_SET(iterClient->second->getSocket(), &fdread);
			}
			// select第一个参数在windows中可以为0,但是在其他系统中需要设置
			int selectRet = select(mMaxSocket + 1, &fdread, NULL, NULL, &tv);
			if (selectRet > 0)
			{
				int selectedClientCount = selectedClient.size();
				FOR(selectedClient, int i = 0; i < selectedClientCount; ++i)
				{
					if (!selectedClient[i]->isDeadClient() && FD_ISSET(selectedClient[i]->getSocket(), &fdread))
					{
						int nRecv = recv(selectedClient[i]->getSocket(), buff, CLIENT_BUFFER_SIZE, 0);
						if (nRecv <= 0)
						{
							if (errno == EPIPE)
							{
								LOG_ERROR("管道损坏错误信号，recv error : EPIPE");
							}
							else if (errno == EAGAIN)
							{
								LOG_ERROR("重试错误信号，recv error : EAGAIN");
							}
							// 客户端可能已经与服务器断开了连接,先立即标记该客户端已断开,然后再移除
							selectedClient[i]->notifyRecvEmpty();
						}
						else
						{
							selectedClient[i]->notifyRecvData(buff, nRecv);
						}
					}
				}
				END(selectedClient);
			}
		}
		END(mClientList);
	}
	catch (std::exception e)
	{
		LOG_ERROR("exception : %s", e.what());
	}
	// 解锁accept列表
	UNLOCK(mClientLock);
}

void NetServer::update(float elapsedTime)
{
	// 更新客户端,找出是否有客户端需要断开连接
	txMap<CLIENT_GUID, NetClient*> tempClientList;
	LOCK(mClientLock);
	tempClientList = mClientList;
	UNLOCK(mClientLock);
	txVector<CLIENT_GUID> logoutClientList;
	auto iterClient = tempClientList.begin();
	auto iterClientEnd = tempClientList.end();
	FOR(tempClientList, ; iterClient != iterClientEnd; ++iterClient)
	{
		iterClient->second->update(elapsedTime);
		// 将已经死亡的客户端放入列表
		if (iterClient->second->isDeadClient())
		{
			logoutClientList.push_back(iterClient->first);
		}
	}
	END(tempClientList);

	int logoutCount = logoutClientList.size();
	FOR(logoutClientList, int i = 0; i < logoutCount; ++i)
	{
		disconnectSocket(logoutClientList[i]);
	}
	END(logoutClientList);
	mCurServerHeartBeatTime += elapsedTime;
	if (mCurServerHeartBeatTime >= mServerHeartBeatTimeout)
	{
		mCurServerHeartBeatTime -= mServerHeartBeatTimeout;
		LOG_INFO("服务器心跳 : %d", mServerHeartBeat++);
	}
}

CLIENT_GUID NetServer::notifyAcceptClient(TX_SOCKET socket, const char* ip)
{
	// 等待解锁accept列表的读写,并锁定accept列表
	CLIENT_GUID clientGUID = 0;
	LOCK(mClientLock);
	clientGUID = generateSocketGUID();
	if (!mClientList.contains(clientGUID))
	{
		NetClient* client = TRACE_NEW(NetClient, client, clientGUID, socket, ip);
		mClientList.insert(clientGUID, client);
		if (mOutputLog)
		{
			LOG_INFO("client : %s connect to server! connect count : %d", ip, (int)mClientList.size());
		}
		if (socket > mMaxSocket)
		{
			mMaxSocket = socket;
		}
	}
	else
	{
		LOG_ERROR("error : client insert to accept list failed!");
	}
	// 解锁accept列表
	UNLOCK(mClientLock);
	return clientGUID;
}

void NetServer::disconnectSocket(CLIENT_GUID client)
{
	// 等待解锁accept列表的读写,并锁定accept列表,将该客户端从接收列表中移除,并且断开该客户端
	LOCK(mClientLock);
	auto iterClient = mClientList.find(client);
	if (iterClient != mClientList.end())
	{
		CHAR_GUID guid = iterClient->second->getCharGUID();
		TRACE_DELETE(iterClient->second);
		mClientList.erase(iterClient);
		if (mOutputLog)
		{
			LOG_INFO("客户端断开连接, GUID : %d, 剩余连接数 : %d", (int)guid, getClientCount());
		}
	}
	// 解锁accept列表
	UNLOCK(mClientLock);
}

NetClient* NetServer::getClient(CLIENT_GUID clientGUID)
{
	NetClient* client = NULL;
	LOCK(mClientLock);
	client = mClientList.tryGet(clientGUID, NULL);
	UNLOCK(mClientLock);
	return client;
}

void NetServer::sendMessage(Packet* packet, CharacterPlayer* player, bool destroyPacketEndSend)
{
	if (player == NULL)
	{
		return;
	}
	sendMessage(packet, player->getClientGUID(), destroyPacketEndSend);
}

void NetServer::sendMessage(Packet* packet, CLIENT_GUID clientGUID, bool destroyPacketEndSend)
{
	if (clientGUID == INVALID_ID)
	{
		return;
	}
	sendMessage(packet, getClient(clientGUID), destroyPacketEndSend);
}

void NetServer::sendMessage(Packet* packet, NetClient* client, bool destroyPacketEndSend)
{
	if (client == NULL)
	{
		return;
	}
	client->sendPacket(packet, destroyPacketEndSend);
}

Packet* NetServer::createPacket(PACKET_TYPE type)
{
	PacketFactoryBase* factory = mPacketFactoryManager->getFactory(type);
	if (factory == NULL)
	{
		LOG_ERROR("error : can not find packet factory : %d", (int)type);
		return NULL;
	}
	return factory->createPacket();
}

void NetServer::destroyPacket(Packet* packet)
{
	if (packet == NULL)
	{
		return;
	}
	PacketFactoryBase* factory = mPacketFactoryManager->getFactory(packet->getPacketType());
	if (factory == NULL)
	{
		LOG_ERROR("error : can not find packet factory : %d", (int)packet->getPacketType());
	}
	factory->destroyPacket(packet);
}

#if RUN_PLATFORM == PLATFORM_LINUX
void NetServer::signalProcess(int signalNum)
{
	if(signalNum == SIGPIPE)
	{
		LOG_INFO("process signal : SINGPIPE");
	}
}
#endif
