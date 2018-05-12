#include "txUtility.h"

#include "GameLog.h"
#include "ServerFramework.h"

void txUtility::stop()
{
	ServerFramework::getSingletonPtr()->stop();
}

void txUtility::sleep(const unsigned long& timeMS)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	Sleep(timeMS);
#elif RUN_PLATFORM == PLATFORM_LINUX
	sleep(timeMS);
#endif
}

unsigned long txUtility::getTimeMS()
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	return timeGetTime();
#elif RUN_PLATFORM == PLATFORM_LINUX
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (unsigned long)(tv.tv_sec * 1000.0f + tv.tv_usec / 1000.0f);
#endif
}

const char* txUtility::getTime()
{
	static char timeBuffer[64] = {0};
#if RUN_PLATFORM == PLATFORM_WINDOWS
	static SYSTEMTIME sys;
	GetLocalTime(&sys);
	SPRINTF(timeBuffer, 64, "%d时%d分%d秒%d毫秒", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
#elif RUN_PLATFORM == PLATFORM_LINUX
	static struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long curMiliTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	const unsigned long& startMiliTime = ServerFramework::getSingletonPtr()->getStartMiliTime();
	unsigned long deltaMiliTime = curMiliTime - startMiliTime;
	unsigned long curSec = deltaMiliTime / 1000;
	unsigned long curMili = deltaMiliTime % 1000;
	SPRINTF(timeBuffer, 64, "%d秒%d毫秒", (int)curSec, (int)curMili);
#endif
	return timeBuffer;
}