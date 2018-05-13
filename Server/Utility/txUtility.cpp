#include "txUtility.h"

#include "GameLog.h"
#include "ServerFramework.h"

ThreadLock txUtility::mTimeLock;

void txUtility::stop()
{
	ServerFramework::getSingletonPtr()->stop();
}

void txUtility::sleep(const unsigned long& timeMS)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	Sleep(timeMS);
#elif RUN_PLATFORM == PLATFORM_LINUX
	usleep(timeMS * 1000);
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
	LOCK(mTimeLock);
	static char timeBuffer[128] = { 0 };
#if RUN_PLATFORM == PLATFORM_WINDOWS
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	SPRINTF(timeBuffer, 128, "%d月%d日%d时%d分%d秒%d毫秒", sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
#elif RUN_PLATFORM == PLATFORM_LINUX
	time_t tt;
	time(&tt);
	struct tm date;
	localtime_r(&tt, &date);
	SPRINTF(timeBuffer, 64, "%d月%d日%d时%d分%d秒", date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
#endif
	UNLOCK(mTimeLock);
	return timeBuffer;
}