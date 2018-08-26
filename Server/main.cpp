#include "ServerDefine.h"
#include "ServerFramework.h"
#include "Utility.h"
#include "TimeLock.h"

#if RUN_PLATFORM == PLATFORM_WINDOWS
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#endif

BOOL ConsoleEventHandler(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_CLOSE_EVENT)
	{
		ServerFramework* mServerFramework = ServerFramework::getSingletonPtr();
		if (mServerFramework != NULL)
		{
			mServerFramework->stop();
			mServerFramework->destroy();
		}
	}
	return FALSE;
}

int main()
{
	// 设置随机数种子
	srand((unsigned int)time(0));
	// 设置窗口事件回调
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleEventHandler, true);

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
