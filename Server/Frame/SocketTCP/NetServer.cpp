#include "NetServer.h"
#include "GameLog.h"
#include "CommandHeader.h"
#include "FrameConfig.h"
#include "PacketFactoryManager.h"
#include "NetClient.h"
#include "TimeLock.h"
#include "CharacterPlayer.h"

CLIENT_GUID NetServer::mSocketGUIDSeed = 1;
float NetServer::mHeartBeatTimeOut = 0.0f;
bool NetServer::mOutputLog = true;
PacketFactoryManager* NetServer::mPacketFactoryManager = NULL;

NetServer::NetServer(const string& name)
:FrameComponent(name)
{
	TRACE_NEW(CustomThread, mReceiveThread, "ReceiveSocket");
	TRACE_NEW(CustomThread, mAcceptThread, "AcceptSocket");
	TRACE_NEW(CustomThread, mParseThread, "ParseSocket");
	mSocket = INVALID_SOCKET;
	mPort = 0;
	mMaxSocket = 0;
	mServerHeartBeatTimeout = 10.0f;
	mCurServerHeartBeatTime = 0.0f;
	mServerHeartBeat = 0;
	TRACE_NEW(PacketFactoryManager, mPacketFactoryManager);
	TRACE_NEW_ARRAY(char, SERVER_RECV_BUFFER_SIZE, mRecvBuffer);
}

void NetServer::destroy()
{
	// 先停止所有子线程
	TRACE_DELETE(mReceiveThread);
	TRACE_DELETE(mAcceptThread);
	TRACE_DELETE(mParseThread);
	// 销毁所有客户端
	auto iterClient = mClientList.begin();
	auto iterClientEnd = mClientList.end();
	FOR(mClientList, ; iterClient != iterClientEnd; ++iterClient)
	{
		TRACE_DELETE(iterClient->second);
	}
	END(mClientList);
	mClientList.clear();
	TRACE_DELETE(mPacketFactoryManager);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	WSACleanup();
#endif
	CLOSE_SOCKET(mSocket);
	TRACE_DELETE_ARRAY(mRecvBuffer);
}

void NetServer::init()
{
#if RUN_PLATFORM == PLATFORM_LINUX
	signal(SIGPIPE, signalProcess);
	signal(SIGBUS, signalProcess);
	signal(SIGSEGV, signalProcess);
#endif
	mPort = (int)mFrameConfig->getFloatParam(FDF_SOCKET_PORT);
	mHeartBeatTimeOut = mFrameConfig->getFloatParam(FDF_HEART_BEAT_TIME_OUT);
	mOutputLog = (int)mFrameConfig->getFloatParam(FDF_OUTPUT_NET_LOG) > 0;
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
	if (listen(mSocket, (int)mFrameConfig->getFloatParam(FDF_BACK_LOG)) != 0)
	{
		mSocket = INVALID_SOCKET;
		LOG_ERROR("listen failed!");
		return;
	}
	mReceiveThread->start(receiveSendSocket, this, 10);
	mAcceptThread->start(acceptSocket, this);
	mParseThread->start(parseSocket, this, 1);
	mParseThread->setBackground(false);
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
		LOG_ERROR("accept failed!");
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
	pushDelayCommand(cmdAccepted, netManager);
	return true;
}

bool NetServer::receiveSendSocket(void* args)
{
	NetServer* netManager = (NetServer*)(args);
	// 有客户端连接到了服务器,才接收数据和发送数据
	if (netManager->getClientCount() > 0)
	{
		LOCK(netManager->mClientRecvLock);
		netManager->processRecv();
		// 处理发送socket消息
		netManager->processSend();
		UNLOCK(netManager->mClientRecvLock);
	}
	return true;
}

void NetServer::processSend()
{
	txVector<NetClient*> selectedClient;
	timeval tv = { 0, 0 };	// select查看后立即返回
	fd_set fdwrite;
	try
	{
		// 克隆一份客户端列表,避免多线程访问容器可能出现的错误
		txMap<CLIENT_GUID, NetClient*> tempList;
		mClientList.clone(tempList);
		auto iterClient = tempList.begin();
		auto iterClientEnd = tempList.end();
		for(; iterClient != iterClientEnd;)
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
				for(int i = 0; i < selectedClientCount; ++i)
				{
					if (FD_ISSET(selectedClient[i]->getSocket(), &fdwrite))
					{
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
			}
		}
	}
	catch (exception e)
	{
		LOG_ERROR("exception : " + string(e.what()));
	}
}

void NetServer::processRecv()
{
	txVector<NetClient*> selectedClient;
	timeval tv = { 0, 0 };	// select查看后立即返回
	fd_set fdread;
	try
	{
		// 克隆一份客户端列表,避免多线程访问容器可能出现的错误
		txMap<CLIENT_GUID, NetClient*> tempList;
		mClientList.clone(tempList);
		auto iterClient = tempList.begin();
		auto iterClientEnd = tempList.end();
		for(; iterClient != iterClientEnd;)
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
				for(int i = 0; i < selectedClientCount; ++i)
				{
					if (!selectedClient[i]->isDeadClient() && FD_ISSET(selectedClient[i]->getSocket(), &fdread))
					{
						int nRecv = recv(selectedClient[i]->getSocket(), mRecvBuffer, SERVER_RECV_BUFFER_SIZE, 0);
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
							selectedClient[i]->notifyRecvData(mRecvBuffer, nRecv);
						}
					}
				}
			}
		}
	}
	catch (exception e)
	{
		LOG_ERROR("exception : " + string(e.what()));
	}
}

bool NetServer::parseSocket(void* args)
{
	NetServer* netManager = (NetServer*)(args);
	// 客户端解析各自的数据
	LOCK(netManager->mClientParseLock);
	// 克隆一份客户端列表,避免多线程访问容器可能出现的错误
	txMap<CLIENT_GUID, NetClient*> tempList;
	netManager->mClientList.clone(tempList);
	auto iterClient = tempList.begin();
	auto iterClientEnd = tempList.end();
	for(; iterClient != iterClientEnd; ++iterClient)
	{
		iterClient->second->parseData();
	}
	UNLOCK(netManager->mClientParseLock);
	return true;
}

void NetServer::update(float elapsedTime)
{
	// 更新客户端,找出是否有客户端需要断开连接
	txVector<CLIENT_GUID> logoutClientList;
	auto iterClient = mClientList.begin();
	auto iterClientEnd = mClientList.end();
	FOR(mClientList, ; iterClient != iterClientEnd; ++iterClient)
	{
		iterClient->second->update(elapsedTime);
		// 将已经死亡的客户端放入列表
		if (iterClient->second->isDeadClient())
		{
			logoutClientList.push_back(iterClient->first);
		}
	}
	END(mClientList);

	// 断开死亡客户端,需要等待所有线程的当前帧都执行完毕,否则在此处直接销毁客户端会导致其他线程报错
	int logoutCount = logoutClientList.size();
	if (logoutCount > 0)
	{
		for(int i = 0; i < logoutCount; ++i)
		{
			disconnectSocket(logoutClientList[i]);
		}
	}

	// 服务器心跳
	mCurServerHeartBeatTime += elapsedTime;
	if (mCurServerHeartBeatTime >= mServerHeartBeatTimeout)
	{
		mCurServerHeartBeatTime -= mServerHeartBeatTimeout;
		LOG_INFO("服务器心跳 : " + intToString(mServerHeartBeat++));
	}
}

CLIENT_GUID NetServer::notifyAcceptClient(TX_SOCKET socket, const char* ip)
{
	CLIENT_GUID clientGUID = 0;
	LOCK(mClientParseLock);
	LOCK(mClientRecvLock);
	clientGUID = generateSocketGUID();
	if (!mClientList.contains(clientGUID))
	{
		NetClient* client = TRACE_NEW(NetClient, client, clientGUID, socket, ip);
		client->init();
		mClientList.insert(clientGUID, client);
		if (mOutputLog)
		{
			LOG_INFO("client : " + string(ip) + " connect to server! connect count : " + intToString(mClientList.size()));
		}
		if (socket > mMaxSocket)
		{
			mMaxSocket = socket;
		}
	}
	else
	{
		LOG_ERROR("client insert to accept list failed!");
	}
	UNLOCK(mClientRecvLock);
	UNLOCK(mClientParseLock);
	return clientGUID;
}

void NetServer::disconnectSocket(CLIENT_GUID client)
{
	LOCK(mClientParseLock);
	LOCK(mClientRecvLock);
	auto iterClient = mClientList.find(client);
	if (iterClient != mClientList.end())
	{
		TRACE_DELETE(iterClient->second);
		mClientList.erase(iterClient);
		if (mOutputLog)
		{
			LOG_INFO("客户端断开连接, 剩余连接数 : " + intToString(getClientCount()));
		}
	}
	UNLOCK(mClientParseLock);
	UNLOCK(mClientRecvLock);
}

NetClient* NetServer::getClient(CLIENT_GUID clientGUID)
{
	NetClient* client = mClientList.tryGet(clientGUID, NULL);
	return client;
}

void NetServer::sendMessage(Packet* packet, IClient* player, bool destroyPacketEndSend)
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
		LOG_ERROR("can not find packet factory : " + intToString(type));
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
		LOG_ERROR("can not find packet factory : " + intToString(packet->getPacketType()));
	}
	factory->destroyPacket(packet);
}

#if RUN_PLATFORM == PLATFORM_LINUX
void NetServer::signalProcess(int signalNum)
{
	if (signalNum == SIGPIPE)
	{
		LOG_INFO("process signal : SIGPIPE");
	}
	else if (signalNum == SIGBUS)
	{
		LOG_INFO("process signal : SIGBUS");
	}
	else if (signalNum == SIGSEGV)
	{
		LOG_INFO("process signal : SIGSEGV");
	}
}
#endif
