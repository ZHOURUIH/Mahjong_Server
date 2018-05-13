#include "Utility.h"
#include "txCommandSystem.h"
#include "txComponentFactory.h"
#include "txComponentFactoryManager.h"
#include "ServerFramework.h"
#include "NetServer.h"
#include "CharacterManager.h"
#include "ServerConfig.h"
#include "ServerBase.h"
#include "RoomManager.h"
#include "MySQLDataBase.h"

template<>ServerFramework *txSingleton<ServerFramework>::ms_Singleton = 0;

ServerFramework::ServerFramework()
:
mStop(false),
mMySQLDataBase(NULL),
mServerConfig(NULL),
mCommandSystem(NULL),
mNetServer(NULL),
mCharacterManager(NULL),
mRoomManager(NULL),
mComponentFactoryManager(NULL)
{
	// 设置随机数种子
	srand((unsigned int)time(0));
#if RUN_PLATFORM == PLATFORM_LINUX
	struct timeval startTime;
	gettimeofday(&startTime, NULL);
	mStartMiliTime = startTime.tv_sec * 1000 + startTime.tv_usec / 1000;
#endif
}

ServerFramework::~ServerFramework()
{
	destroy();
}

bool ServerFramework::init()
{
	// 创建所有组件
	mComponentFactoryManager = TRACE_NEW(txComponentFactoryManager, mComponentFactoryManager);
	mServerConfig = TRACE_NEW(ServerConfig, mServerConfig);
	mCharacterManager = TRACE_NEW(CharacterManager, mCharacterManager);
	mCommandSystem = TRACE_NEW(txCommandSystem, mCommandSystem);
	mNetServer = TRACE_NEW(NetServer, mNetServer);
	mRoomManager = TRACE_NEW(RoomManager, mRoomManager);
	mMySQLDataBase = TRACE_NEW(MySQLDataBase, mMySQLDataBase);
	ServerBase::notifyConstructDone();

	// 初始化所有组件
	initComponentFactory();
	mServerConfig->init();
	mMySQLDataBase->init("root", "zhourui", "localhost", 3306);
	//mMySQLDataBase->init("root", "zhourui", ServerConfig::getStringParam(SDS_DOMAIN_NAME), 3306);
	mCharacterManager->init();
	mCommandSystem->init();
	mNetServer->init((int)ServerConfig::getFloatParam(SDF_SOCKET_PORT), (int)ServerConfig::getFloatParam(SDF_BACK_LOG));
	mRoomManager->init();
	return true;
}

void ServerFramework::update(const float& elapsedTime)
{
	mMySQLDataBase->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
	mCharacterManager->update(elapsedTime);
	mNetServer->update(elapsedTime);
	mRoomManager->update(elapsedTime);
}

void ServerFramework::destroy()
{
	TRACE_DELETE(mRoomManager);
	TRACE_DELETE(mNetServer);
	TRACE_DELETE(mCharacterManager);
	TRACE_DELETE(mCommandSystem);
	TRACE_DELETE(mServerConfig);
	TRACE_DELETE(mComponentFactoryManager);
	TRACE_DELETE(mMySQLDataBase);
	destroyComponentFactory();
	LOG_INFO("%s | 关闭服务器！", txUtility::getTime());
}

void ServerFramework::launch()
{
	if (mNetServer != NULL)
	{
		LOG_INFO("%s | 启动服务器,端口 : %d", txUtility::getTime(), mNetServer->getPort());
	}
	else
	{
		LOG_INFO("启动服务器失败!");
	}
}

void ServerFramework::initComponentFactory()
{
	// 基本组件类型
}

void ServerFramework::destroyComponentFactory()
{
	;
}
