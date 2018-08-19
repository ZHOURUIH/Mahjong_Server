#include "Utility.h"
#include "txCommandSystem.h"
#include "txComponentFactory.h"
#include "ServerFramework.h"
#include "ServerBase.h"

template<>ServerFramework *txSingleton<ServerFramework>::ms_Singleton = 0;

ServerFramework::ServerFramework()
:
mStop(false)
{
#if RUN_PLATFORM == PLATFORM_LINUX
	struct timeval startTime;
	gettimeofday(&startTime, NULL);
	mStartMiliTime = startTime.tv_sec * 1000 + startTime.tv_usec / 1000;
#endif
	REGISTE_FRAME_COMPONENT(ServerConfig);
	REGISTE_FRAME_COMPONENT(CharacterManager);
	REGISTE_FRAME_COMPONENT(txCommandSystem);
	REGISTE_FRAME_COMPONENT(NetServer);
	REGISTE_FRAME_COMPONENT(RoomManager);
	REGISTE_FRAME_COMPONENT(MySQLDataBase);
	REGISTE_FRAME_COMPONENT(txComponentFactoryManager);
	REGISTE_FRAME_COMPONENT(DataBase);
	REGISTE_FRAME_COMPONENT(GameLog);
	REGISTE_FRAME_COMPONENT(txMemoryTrace);
	ServerBase::notifyConstructDone();
	LOG_ERROR("1");
}

ServerFramework::~ServerFramework()
{
	destroy();
}

bool ServerFramework::init()
{
	initComponentFactory();
	// 初始化所有组件
	int count = mFrameComponentVector.size();
	FOR_STL(mFrameComponentVector, int i = 0; i < count; ++i)
	{
		mFrameComponentVector[i]->init();
	}
	END_FOR_STL(mFrameComponentVector);
	return true;
}

void ServerFramework::update(float elapsedTime)
{
	int count = mFrameComponentVector.size();
	FOR_STL(mFrameComponentVector, int i = 0; i < count; ++i)
	{
		mFrameComponentVector[i]->update(elapsedTime);
	}
	END_FOR_STL(mFrameComponentVector);
}

void ServerFramework::destroy()
{
	int count = mFrameComponentVector.size();
	FOR_STL(mFrameComponentVector, int i = 0; i < count; ++i)
	{
		TRACE_DELETE(mFrameComponentVector[i]);
	}
	END_FOR_STL(mFrameComponentVector);
	mFrameComponentVector.clear();
	mFrameComponentMap.clear();
	destroyComponentFactory();
	LOG_INFO("关闭服务器!");
}

void ServerFramework::launch()
{
	NetServer* server = GET_SYSTEM(NetServer);
	if (server != NULL)
	{
		LOG_INFO("启动服务器,端口 : %d", server->getPort());
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
