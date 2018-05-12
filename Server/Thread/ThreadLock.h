#ifndef _THREAD_LOCK_H_
#define _THREAD_LOCK_H_

#include "ServerDefine.h"

class ThreadLock
{
public:
	ThreadLock()
	{
		mLock = 0;
		mLine = 0;
		mThreadID = 0;
	}
	~ThreadLock(){}
	void waitForUnlock(const char* file, const int& line);
	void unlock();
	bool isLocked(){ return mLock != 0; }
public:
	volatile unsigned int mLock;	// 1��ʾ����,0��ʾδ����
	volatile char mFile[256];		// �������ļ���
	volatile int mLine;				// �������к�
	volatile int mThreadID;			// �����̵߳�ID
};

#endif