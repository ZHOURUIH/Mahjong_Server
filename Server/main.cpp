﻿#include "ServerDefine.h"
#include "ServerFramework.h"
#include "Utility.h"
#include "TimeLock.h"

#if RUN_PLATFORM == PLATFORM_WINDOWS
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#endif

int main()
{
	// 设置随机数种子
	srand((unsigned int)time(0));

	TimeLock timeLock(30);
	ServerFramework* mServerFramework = TRACE_NEW(ServerFramework, mServerFramework);
	mServerFramework->init();
	// 创建完窗口后启动客户端
	mServerFramework->launch();
	while (!mServerFramework->isStop())
	{
		long frameTime = timeLock.update();
		mServerFramework->update(frameTime / 1000.0f);
	}
	TRACE_DELETE(mServerFramework);
	return 0;
}
