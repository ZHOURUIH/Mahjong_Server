#ifndef _SERVER_FRAMEWORK_H_
#define _SERVER_FRAMEWORK_H_

#include "txSingleton.h"
#include "ThreadLock.h"

class FrameComponent;
class ServerFramework : public txSingleton<ServerFramework>
{
public:
	ServerFramework();
	virtual ~ServerFramework();
	virtual bool init();
	virtual void update(float elapsedTime);
	void destroy();
	virtual void launch();
	bool isStop() { return mStop; }
	void stop() { mStop = true; }
	// 获得成员变量
#if RUN_PLATFORM == PLATFORM_LINUX
	const unsigned long& getStartMiliTime() {return mStartMiliTime;}
#endif
	template<typename T>
	T* getSystem(const string& name)
	{
		return static_cast<T*>(mFrameComponentMap.tryGet(name, NULL));
	}
	template<typename T>
	void getSystem(const string& name, T*& component)
	{
		component = static_cast<T*>(mFrameComponentMap.tryGet(name, NULL));
	}
protected:
	void initComponentFactory();
	void destroyComponentFactory();
	virtual void registeComponent();
	virtual void notifyBase(bool construct);
protected:
#if RUN_PLATFORM == PLATFORM_LINUX
	unsigned long mStartMiliTime;
#endif
	atomic<bool> mStop;
	ThreadLock mLock;
	txVector<FrameComponent*> mFrameComponentVector;
	txMap<string, FrameComponent*> mFrameComponentMap;
};

#endif
