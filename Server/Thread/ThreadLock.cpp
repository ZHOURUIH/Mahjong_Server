﻿#include "ThreadLock.h"
#include "Utility.h"
#include "GameLog.h"

void ThreadLock::waitForUnlock(const char* file, int line)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	int threadID = GetCurrentThreadId();
#elif RUN_PLATFORM == PLATFORM_ANDROID
	int threadID = pthread_self();
#endif
	if (isLocked() && threadID == mThreadID)
	{
		LOG_ERROR("can not lock in the same thread!");
	}
	// 暂不区分读锁定和写锁定
	while (mLock.exchange(1) == 1)
	{
		txUtility::sleep(1);
	}
	memset((char*)mFile, 0, 256);
	memcpy((char*)mFile, file, strlen(file));
	mLine = line;
	mThreadID = threadID;
}

void ThreadLock::unlock()
{
	mLock.exchange(0);
}