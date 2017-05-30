#ifndef _TX_THREAD_LOCK_H_
#define _TX_THREAD_LOCK_H_

#include "ServerDefine.h"

class txThreadLock
{
public:
	txThreadLock()
	{
		//mLock = false;
		mLine = 0;
		mRead = LT_WRITE;
		mReadLockCount = 0;
		mThreadID = 0;
		mShowLockError = 0;
#if RUN_PLATFORM == PLATFORM_WINDOWS
		mMutex = CreateMutexA(NULL, FALSE, NULL);
#elif RUN_PLATFORM == PLATFORM_LINUX
		mMutex = PTHREAD_MUTEX_INITIALIZER;
#endif
	}
	virtual ~txThreadLock()
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		CloseHandle(mMutex);
#elif RUN_PLATFORM == PLATFORM_LINUX
		pthread_mutex_destroy(&mMutex);
#endif
	}
	void waitForUnlock(const char*file, const int& line, const LOCK_TYPE& read, const bool& showDebug = false);
	void unlock(const LOCK_TYPE& read, const bool& showDebug = false);
public:
	//volatile bool mLock;
	volatile char mFile[256];
	volatile int mLine;
	volatile int mThreadID;			// 加锁线程的ID
	volatile LOCK_TYPE mRead;		// 锁定后是想要读(true)或者写(false)
	volatile int mReadLockCount;	// 锁定读的次数,当锁定读的次数为0时才能完全解锁
	volatile int mShowLockError;
#if RUN_PLATFORM == PLATFORM_WINDOWS
	volatile HANDLE mMutex;
#elif RUN_PLATFORM == PLATFORM_LINUX
	pthread_mutex_t mMutex;
#endif
};

#endif
