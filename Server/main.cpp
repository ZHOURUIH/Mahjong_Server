#include "ServerDefine.h"

#if RUN_PLATFORM == PLATFORM_WINDOWS
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#endif

#include "txMemeryTrace.h"
#include "ServerFramework.h"

int main()
{
#ifdef _TRACE_MEMERY
	MemeryTrace* trace = TRACE_NEW(MemeryTrace, trace);
	trace->init(true);
#endif

	ServerFramework* mServerFramework = TRACE_NEW(ServerFramework, mServerFramework);
	mServerFramework->init();
	// 创建完窗口后启动客户端
	mServerFramework->launch();
	unsigned long beginTime = 0;
	unsigned long deltaTime = 0;
#if RUN_PLATFORM == PLATFORM_WINDOWS
	static unsigned long lastBeginTime = timeGetTime();
#elif RUN_PLATFORM == PLATFORM_LINUX
	struct timeval tv;
	gettimeofday(&tv, NULL);
	static unsigned long lastBeginTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
	while (!mServerFramework->isStop())
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		beginTime = timeGetTime();
#elif RUN_PLATFORM == PLATFORM_LINUX
		gettimeofday(&tv, NULL);
		beginTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
		deltaTime = beginTime - lastBeginTime;
		mServerFramework->update(deltaTime / 1000.0f);
		lastBeginTime = beginTime;
		Sleep(20);
	}

	TRACE_DELETE(mServerFramework);

#ifdef _TRACE_MEMERY
	TRACE_DELETE(trace);
#endif

	return 0;
}
