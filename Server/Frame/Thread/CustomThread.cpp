#include "CustomThread.h"
#include "Utility.h"
#include "txMemoryTrace.h"
#include "TimeLock.h"

CustomThread::CustomThread(const std::string& name)
{
	mName = name;
	mFinish = true;
	mRunning = false;
	mCallback = NULL;
	mThread = NULL_THREAD;
	mTimeLock = NULL;
	mPause = false;
	mIsBackground = true;
	mArgs = NULL;
}

CustomThread::~CustomThread()
{
	destroy();
}

void CustomThread::destroy()
{
	stop();
}

void CustomThread::start(CustomThreadCallback callback, void* args, int frameTimeMS, int forceSleep)
{
	if (mThread != NULL_THREAD)
	{
		LOG_ERROR("线程已经启动 : " + mName);
		return;
	}
	TRACE_NEW(TimeLock, mTimeLock, frameTimeMS, forceSleep);
	mRunning = true;
	mCallback = callback;
	mArgs = args;
	CREATE_THREAD(mThread, run, this);
	LOG_INFO("线程启动成功 : " + mName);
}

void CustomThread::stop()
{
	mRunning = false;
	mPause = false;
	while (!mIsBackground && !mFinish) {}
	CLOSE_THREAD(mThread);
	mCallback = NULL;
	TRACE_DELETE(mTimeLock);
	LOG_INFO("线程退出完成! 线程名 : " + mName);
}

void CustomThread::updateThread()
{
	mFinish = false;
	while (mRunning)
	{
		mTimeLock->update();
		try
		{
			if (mPause)
			{
				continue;
			}
			if (!mCallback(mArgs))
			{
				break;
			}
		}
		catch (...)
		{
			LOG_INFO("捕获线程异常! 线程名 : " + mName);
		}
	}
	mFinish = true;
}