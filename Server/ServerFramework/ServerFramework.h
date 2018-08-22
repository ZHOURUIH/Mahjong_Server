#ifndef _SERVER_FRAMEWORK_H_
#define _SERVER_FRAMEWORK_H_

#include "txSingleton.h"
#include "ServerConfig.h"
#include "CharacterManager.h"
#include "txCommandSystem.h"
#include "NetServer.h"
#include "RoomManager.h"
#include "MySQLDataBase.h"
#include "txComponentFactoryManager.h"
#include "MahjongRobotManager.h"
#include "DataBase.h"
#include "GameLog.h"

#define GET_SYSTEM(type) getSystem<type>(TOSTRING(type))

#define REGISTE_FRAME_COMPONENT(type)								\
	{type* component = TRACE_NEW(type, component, TOSTRING(type));	\
	mFrameComponentVector.push_back(component);						\
	mFrameComponentMap.insert(TOSTRING(type), component);}

class FrameComponent;
class ServerFramework : public txSingleton<ServerFramework>
{
public:
	ServerFramework();
	virtual ~ServerFramework();
	bool init();
	void update(float elapsedTime);
	void destroy();
	void launch();
	bool isStop() { return mStop; }
	void stop() { mStop = true; }
	// 获得成员变量
#if RUN_PLATFORM == PLATFORM_LINUX
	const unsigned long& getStartMiliTime() {return mStartMiliTime;}
#endif
	template<typename T>
	T* getSystem(const std::string& name)
	{
		return static_cast<T*>(mFrameComponentMap.tryGet(name, NULL));
	}
protected:
	void initComponentFactory();
	void destroyComponentFactory();
protected:
#if RUN_PLATFORM == PLATFORM_LINUX
	unsigned long mStartMiliTime;
#endif
	bool mStop;
	txVector<FrameComponent*> mFrameComponentVector;
	txMap<std::string, FrameComponent*> mFrameComponentMap;
};

#endif
