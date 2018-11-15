#include "Utility.h"
#include "txComponentFactory.h"
#include "ServerFramework.h"
#include "FrameConfig.h"
#include "CharacterManager.h"
#include "CommandSystem.h"
#include "NetServer.h"
#include "ComponentFactoryManager.h"
#include "GameLog.h"
#include "InputSystem.h"

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
}

void ServerFramework::registeComponent()
{
	REGISTE_FRAME_COMPONENT(FrameConfig);
	REGISTE_FRAME_COMPONENT(GameLog);
	REGISTE_FRAME_COMPONENT(CharacterManager);
	REGISTE_FRAME_COMPONENT(CommandSystem);
	REGISTE_FRAME_COMPONENT(NetServer);
	REGISTE_FRAME_COMPONENT(ComponentFactoryManager);
	REGISTE_FRAME_COMPONENT(txMemoryTrace);
}

void ServerFramework::notifyBase(bool construct)
{
	FrameBase* frameBase = TRACE_NEW(FrameBase, frameBase);
	if (construct)
	{
		frameBase->notifyConstructDone();
	}
	else
	{
		frameBase->notifyComponentDeconstruct();
	}
	TRACE_DELETE(frameBase);
}

ServerFramework::~ServerFramework()
{
	destroy();
}

bool ServerFramework::init()
{
	registeComponent();
	// 输入系统需要最后注册
	REGISTE_FRAME_COMPONENT(InputSystem);
	notifyBase(true);
	initComponentFactory();
	// 初始化所有组件
	int count = mFrameComponentVector.size();
	FOR(mFrameComponentVector, int i = 0; i < count; ++i)
	{
		mFrameComponentVector[i]->init();
	}
	END(mFrameComponentVector);
	return true;
}

void ServerFramework::update(float elapsedTime)
{
	LOCK(mLock);
	int count = mFrameComponentVector.size();
	FOR(mFrameComponentVector, int i = 0; i < count; ++i)
	{
		mFrameComponentVector[i]->update(elapsedTime);
	}
	END(mFrameComponentVector);
	UNLOCK(mLock);
}

void ServerFramework::destroy()
{
	LOCK(mLock);
	int count = mFrameComponentVector.size();
	FOR(mFrameComponentVector, int i = 0; i < count; ++i)
	{
		// 销毁顺序与初始化顺序相反
		string componentName = mFrameComponentVector[count - 1 - i]->getName();
		TRACE_DELETE(mFrameComponentVector[count - 1 - i]);
		mFrameComponentMap[componentName] = NULL;
		notifyBase(false);
	}
	END(mFrameComponentVector);
	mFrameComponentVector.clear();
	mFrameComponentMap.clear();
	destroyComponentFactory();
	LOG_INFO("关闭服务器!");
	UNLOCK(mLock);
}

void ServerFramework::launch()
{
	if (FrameBase::mNetServer != NULL && FrameBase::mNetServer->isAvailable())
	{
		LOG_INFO("启动服务器, 端口 : " + StringUtility::intToString(FrameBase::mNetServer->getPort()));
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
