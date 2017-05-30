#include "txThreadLock.h"
#include "GameLog.h"

void txThreadLock::waitForUnlock(const char* file, const int& line, const LOCK_TYPE& read, const bool& showDebug)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	WaitForSingleObject(mMutex, INFINITE);
#elif RUN_PLATFORM == PLATFORM_LINUX
	pthread_mutex_lock(&mMutex);
#endif

#if RUN_PLATFORM == PLATFORM_WINDOWS
	int threadID = GetCurrentThreadId();
#elif RUN_PLATFORM == PLATFORM_LINUX
	int threadID = 0;
#endif
	//// 只有当前是写锁定并且想要再写锁定时才不需要等待解锁,其余情况都需要等待解锁
	//if (read != LT_READ || mRead != LT_READ)
	//{
	//	while (mLock)
	//	{}
	//	// 解锁后立即上锁
	//	mLock = true;
	//}
	//mLock = true;
	memset((void*)mFile, 0, 256);
	memcpy((void*)mFile, file, strlen(file));
	mLine = line;
	mThreadID = threadID;
	mRead = read;
	//if (read == LT_READ)
	//{
	//	++mReadLockCount;
	//	if (mReadLockCount <= 0 && mShowLockError < 2)
	//	{
	//		++mShowLockError;
	//		LOG_INFO("lock : %s, %d, read : %s", mFile, mLine, mRead == LT_READ ? "true" : "false");
	//		GAME_ERROR("error : read lock count error! read lock : %d", mReadLockCount);
	//	}
	//}

	if (showDebug)
	{
		LOG_INFO("lock : %s, %d, read : %s, read count : %d", mFile, mLine, mRead == LT_READ ? "true" : "false", mReadLockCount);
	}
}

void txThreadLock::unlock(const LOCK_TYPE& read, const bool& showDebug)
{
	//// 读取锁定时,解锁只是减少锁定计数
	//if (read == LT_READ)
	//{
	//	--mReadLockCount;
	//	mLock = mReadLockCount > 0;
	//	if (mReadLockCount < 0 && mShowLockError < 2)
	//	{
	//		++mShowLockError;
	//		LOG_INFO("lock : %s, %d, read : %s", mFile, mLine, mRead == LT_READ ? "true" : "false");
	//		GAME_ERROR("error : thread read lock error! read lock : %d", mReadLockCount);
	//	}
	//}
	//// 写入锁定时,由于不可读写,所以可完全解锁
	//else if (read == LT_WRITE)
	//{
	//	mLock = false;
	//}
	if (showDebug)
	{
		LOG_INFO("unlock : %s, %d, read : %s, read count : %d", mFile, mLine, mRead == LT_READ ? "true" : "false", mReadLockCount);
	}

#if RUN_PLATFORM == PLATFORM_WINDOWS
	ReleaseMutex(mMutex);
#elif RUN_PLATFORM == PLATFORM_LINUX
	pthread_mutex_unlock(&mMutex);
#endif
}
