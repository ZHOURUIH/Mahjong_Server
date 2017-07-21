﻿#include "NetServer.h"
#include "GameLog.h"
#include "CommandHeader.h"
#include "ServerConfig.h"
#include "PacketFactoryManager.h"
#include "NetClient.h"

CLIENT_GUID NetServer::mSocketGUIDSeed = 0;
PacketFactoryManager* NetServer::mPacketFactoryManager = NULL;
float NetServer::mHeartBeatTimeOut = 0.0f;
bool NetServer::mOutputLog = true;

NetServer::NetServer()
:
txCommandReceiver("NetManagerServer")
{
	mReceiveThread = NULL_THREAD;
	mAcceptThread = NULL_THREAD;
	mSocket = INVALID_SOCKET;
	mPort = 0;
	mBackLog = 0;
	mOutputLog = true;
	mMaxSocket = 0;
	mHeartBeatTimeOut = 0.0f;
	mPacketFactoryManager = NULL;
}

void NetServer::destroy()
{
	txMap<CLIENT_GUID, NetClient*>::iterator iterClient = mClientList.begin();
	txMap<CLIENT_GUID, NetClient*>::iterator iterClientEnd = mClientList.end();
	FOR_STL(mClientList, ; iterClient != iterClientEnd; ++iterClient)
	{
		TRACE_DELETE(iterClient->second);
	}
	END_FOR_STL(mClientList);
	mClientList.clear();
	TRACE_DELETE(mPacketFactoryManager);

#if RUN_PLATFORM == PLATFORM_WINDOWS
	if (mReceiveThread != NULL_THREAD)
	{
		TerminateThread(mReceiveThread, 0);
		CloseHandle(mReceiveThread);
		mReceiveThread = NULL_THREAD;
	}
	if (mAcceptThread != NULL_THREAD)
	{
		TerminateThread(mAcceptThread, 0);
		CloseHandle(mAcceptThread);
		mAcceptThread = NULL_THREAD;
	}
	WSACleanup();
	closesocket(mSocket);
#elif RUN_PLATFORM == PLATFORM_LINUX
	if (mReceiveThread != NULL_THREAD)
	{
		pthread_cancel(mReceiveThread);
		mReceiveThread = NULL_THREAD;
	}
	if (mAcceptThread != NULL_THREAD)
	{
		pthread_cancel(mAcceptThread);
		mAcceptThread = NULL_THREAD;
	}
	close(mSocket);
#endif
}

void NetServer::init(int port, int backLog)
{
#if RUN_PLATFORM == PLATFORM_LINUX
	signal(SIGPIPE, signalProcess);
#endif

	mPort = port;
	mBackLog = backLog;

	mHeartBeatTimeOut = ServerConfig::getFloatParam(SD_HEART_BEAT_TIME_OUT);
	mOutputLog = (int)ServerConfig::getFloatParam(SD_OUTPUT_NET_LOG) > 0;
	// 初始化工厂
	mPacketFactoryManager = TRACE_NEW(PacketFactoryManager, mPacketFactoryManager);
	mPacketFactoryManager->init();

	//初始化Socket环境
#if RUN_PLATFORM == PLATFORM_WINDOWS
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		GAME_ERROR("WSAStartup failed!");
		return;
	}
#endif
	//创建监听的Socket
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == (TX_SOCKET)INVALID_SOCKET)
	{
		GAME_ERROR("socket failed!");
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
		GAME_ERROR("bind failed!");
		return;
	}
	//在Sockets Server上进行监听
	if (listen(mSocket, mBackLog) != 0)
	{
		GAME_ERROR("listen failed!");
		return;
	}
#if RUN_PLATFORM == PLATFORM_WINDOWS
	mReceiveThread = CreateThread(NULL, 0, NetServer::receiveSendSocket, this, 0, NULL);
	mAcceptThread = CreateThread(NULL, 0, NetServer::acceptSocket, this, 0, NULL);
#elif RUN_PLATFORM == PLATFORM_LINUX
	pthread_create(&mReceiveThread, NULL, NetManagerServer::receiveSendSocket, this);
	pthread_create(&mAcceptThread, NULL, NetManagerServer::acceptSocket, this);
#endif
}

#if RUN_PLATFORM == PLATFORM_WINDOWS
DWORD WINAPI NetServer::acceptSocket(LPVOID lpParameter)
#elif RUN_PLATFORM == PLATFORM_LINUX
void* NetServer::acceptSocket(void* args)
#endif
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	NetServer* netManager = (NetServer*)(lpParameter);
#elif RUN_PLATFORM == PLATFORM_LINUX
	NetServer* netManager = (NetServer*)(args);
#endif
	sockaddr_in addr;
#if RUN_PLATFORM == PLATFORM_WINDOWS
	int nLen = sizeof(addr);
#elif RUN_PLATFORM == PLATFORM_LINUX
	socklen_t nLen = sizeof(addr);
#endif
	char ip[16] = { 0 };
	while (true)
	{
		TX_SOCKET sClient = accept(netManager->mSocket, (struct sockaddr*)&addr, &nLen);
		if (sClient == (TX_SOCKET)INVALID_SOCKET)
		{
			GAME_ERROR("error : accept failed!");
			return 0;
		}
		// 获得客户端IP,然后通知已经接收到一个客户端的连接
		if (netManager->mOutputLog)
		{
#if RUN_PLATFORM == PLATFORM_WINDOWS
			SPRINTF(ip, 16, "%d.%d.%d.%d", addr.sin_addr.S_un.S_un_b.s_b1, addr.sin_addr.S_un.S_un_b.s_b2, addr.sin_addr.S_un.S_un_b.s_b3, addr.sin_addr.S_un.S_un_b.s_b4);
#elif RUN_PLATFORM == PLATFORM_LINUX
			int ip0 = GET_BYTE(addr.sin_addr.s_addr, 0);
			int ip1 = GET_BYTE(addr.sin_addr.s_addr, 1);
			int ip2 = GET_BYTE(addr.sin_addr.s_addr, 2);
			int ip3 = GET_BYTE(addr.sin_addr.s_addr, 3);
			SPRINTF(ip, 16, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);
#endif
		}
		CommandServerNotifyAcceptedClient* notifyAccepted = txCommand::createDelayCommand<CommandServerNotifyAcceptedClient>(CMD_PARAM, false);
		notifyAccepted->mSocket = sClient;
		notifyAccepted->mIP = ip;
		mCommandSystem->pushDelayCommand(notifyAccepted, netManager);
	}
	return 0;
}

#if RUN_PLATFORM == PLATFORM_WINDOWS
DWORD WINAPI NetServer::receiveSendSocket(LPVOID lpParameter)
#elif RUN_PLATFORM == PLATFORM_LINUX
void* NetServer::receiveSendSocket(void* args)
#endif
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	NetServer* netManager = (NetServer*)(lpParameter);
#elif RUN_PLATFORM == PLATFORM_LINUX
	NetServer* netManager = (NetServer*)(args);
#endif
	while (true)
	{
		// 有客户端连接到了服务器,才接收数据和发送数据
		if (netManager->getClientCount() > 0)
		{
			netManager->processRecv();
			// 处理发送socket消息
			netManager->processSend();
		}
	}
	return 0;
}

void NetServer::processSend()
{
	txVector<NetClient*> selectedClient;
	timeval tv = { 0, 0 };	// select查看后立即返回
	fd_set fdwrite;
	// 等待发送列表解锁,然后锁定发送列表
	LOCK(mClientLock, LT_READ);
	txMap<CLIENT_GUID, NetClient*>::iterator iterClient = mClientList.begin();
	txMap<CLIENT_GUID, NetClient*>::iterator iterClientEnd = mClientList.end();
	FOR_STL(mClientList, ; iterClient != iterClientEnd;)
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
			FOR_STL(selectedClient, int i = 0; i < selectedClientCount; ++i)
			{
				if (FD_ISSET(selectedClient[i]->getSocket(), &fdwrite))
				{
					if (mOutputLog)
					{
						LOG_INFO("%s | send to client : %s", txUtility::getTime(), selectedClient[i]->getIP());
					}
					int sendedCount = send(selectedClient[i]->getSocket(), selectedClient[i]->getSendData(), selectedClient[i]->getSendDataCount(), 0);
					// 检查是否send有错误
					if (sendedCount < 0)
					{
						if (errno == EPIPE)
						{
							GAME_ERROR("%s | 管道损坏错误信号，send error : EPIPE", txUtility::getTime());
						}
						else if (errno == EAGAIN)
						{
							GAME_ERROR("%s | 重试错误信号，send error : EAGAIN", txUtility::getTime());
						}
					}
					else
					{
						selectedClient[i]->notifyDataSended(sendedCount);
					}
				}
			}
			END_FOR_STL(selectedClient);
		}
	}
	END_FOR_STL(mClientList);

	// 解锁发送列表
	UNLOCK(mClientLock, LT_READ);
}

void NetServer::processRecv()
{
	char buff[CLIENT_BUFFER_SIZE];
	txVector<NetClient*> selectedClient;
	timeval tv = { 0, 0 };	// select查看后立即返回
	fd_set fdread;
	// 等待解锁accept列表的读写,并锁定accept列表
	LOCK(mClientLock, LT_READ);
	txMap<CLIENT_GUID, NetClient*>::iterator iterClient = mClientList.begin();
	txMap<CLIENT_GUID, NetClient*>::iterator iterClientEnd = mClientList.end();
	FOR_STL(mClientList, ; iterClient != iterClientEnd;)
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
			FOR_STL(selectedClient, int i = 0; i < selectedClientCount; ++i)
			{
				if (!selectedClient[i]->isDeadClient() && FD_ISSET(selectedClient[i]->getSocket(), &fdread))
				{
					int nRecv = recv(selectedClient[i]->getSocket(), buff, CLIENT_BUFFER_SIZE, 0);
					if (nRecv <= 0)
					{
						if (errno == EPIPE)
						{
							GAME_ERROR("%s | 管道损坏错误信号，recv error : EPIPE", txUtility::getTime());
						}
						else if (errno == EAGAIN)
						{
							GAME_ERROR("%s | 重试错误信号，recv error : EAGAIN", txUtility::getTime());
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
			END_FOR_STL(selectedClient);
		}
	}
	END_FOR_STL(mClientList);

	// 解锁accept列表
	UNLOCK(mClientLock, LT_READ);
}

void NetServer::update(const float& elapsedTime)
{
	// 更新客户端,找出是否有客户端需要断开连接
	LOCK(mClientLock, LT_READ);
	txVector<CLIENT_GUID> logoutClientList;
	txMap<CLIENT_GUID, NetClient*>::iterator iterClient = mClientList.begin();
	txMap<CLIENT_GUID, NetClient*>::iterator iterClientEnd = mClientList.end();
	FOR_STL(mClientList, ; iterClient != iterClientEnd; ++iterClient)
	{
		iterClient->second->update(elapsedTime);
		// 将已经死亡的客户端放入列表
		if (iterClient->second->isDeadClient())
		{
			logoutClientList.push_back(iterClient->first);
		}
	}
	END_FOR_STL(mClientList);
	UNLOCK(mClientLock, LT_READ);

	int logoutCount = logoutClientList.size();
	FOR_STL(logoutClientList, int i = 0; i < logoutCount; ++i)
	{
		disconnectSocket(logoutClientList[i]);
	}
	END_FOR_STL(logoutClientList);
}

CLIENT_GUID NetServer::notifyAcceptClient(const TX_SOCKET& socket, const char* ip)
{
	// 等待解锁accept列表的读写,并锁定accept列表
	LOCK(mClientLock, LT_WRITE);
	CLIENT_GUID clientGUID = generateSocketGUID();
	if (mClientList.find(clientGUID) == mClientList.end())
	{
		NetClient* client = TRACE_NEW(NetClient, client, clientGUID, socket, ip);
		mClientList.insert(clientGUID, client);
		if (mOutputLog)
		{
			LOG_INFO("%s | client : %s connect to server! connect count : %d", txUtility::getTime(), ip, (int)mClientList.size());
		}
		if (socket > mMaxSocket)
		{
			mMaxSocket = socket;
		}
	}
	else
	{
		GAME_ERROR("error : client insert to accept list failed!");
	}
	// 解锁accept列表
	UNLOCK(mClientLock, LT_WRITE);
	return clientGUID;
}

void NetServer::disconnectSocket(const CLIENT_GUID& client)
{
	// 等待解锁accept列表的读写,并锁定accept列表,将该客户端从接收列表中移除,并且断开该客户端
	LOCK(mClientLock, LT_WRITE);
	txMap<CLIENT_GUID, NetClient*>::iterator iterClient = mClientList.find(client);
	if (iterClient != mClientList.end())
	{
		const CHAR_GUID& guid = iterClient->second->getCharGUID();
		TRACE_DELETE(iterClient->second);
		mClientList.erase(iterClient);
		if (mOutputLog)
		{
			LOG_INFO("客户端断开连接, GUID : %d, 剩余连接数 : %d", (int)guid, getClientCount());
		}
	}
	// 解锁accept列表
	UNLOCK(mClientLock, LT_WRITE);
}

void NetServer::sendMessage(Packet* packet, const CLIENT_GUID& clientGUID, const bool& destroyPacketEndSend)
{
	sendMessage(packet, getClient(clientGUID), destroyPacketEndSend);
}

void NetServer::sendMessage(Packet* packet, NetClient* client, const bool& destroyPacketEndSend)
{
	if (client == NULL)
	{
		return;
	}
	client->sendPacket(packet, destroyPacketEndSend);
}

Packet* NetServer::createPacket(const PACKET_TYPE& type)
{
	PacketFactoryBase* factory = mPacketFactoryManager->getFactory(type);
	if (factory == NULL)
	{
		GAME_ERROR("error : can not find packet factory : %d", (int)type);
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
		GAME_ERROR("error : can not find packet factory : %d", (int)packet->getPacketType());
	}
	factory->destroyPacket(packet);
}

#if RUN_PLATFORM == PLATFORM_LINUX
void NetManagerServer::signalProcess(int signalNum)
{
	if(signalNum == SIGPIPE)
	{
		LOG_INFO("%s | process signal : SINGPIPE", txUtility::getTime());
	}
}
#endif
