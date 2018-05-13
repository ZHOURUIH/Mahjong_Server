#include "CustomThread.h"
#include "txUtility.h"
#include "txMemoryTrace.h"
#include "TimeLock.h"
#include "GameLog.h"
#include "txStringUtility.h"

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
}

CustomThread::~CustomThread()
{
	destroy();
}

void CustomThread::destroy()
{
	stop();
}

void CustomThread::start(CustomThreadCallback callback, void* args, const int& frameTimeMS)
{
	LOG_INFO("׼�������߳� : %s", mName.c_str());
	if (mThread != NULL_THREAD)
	{
		LOG_ERROR("�߳��Ѿ����� : %s", mName.c_str());
		return;
	}
	mTimeLock = TRACE_NEW(TimeLock, mTimeLock, frameTimeMS);
	mTimeLock->setForceSleep(5);
	mRunning = true;
	mCallback = callback;
	mArgs = args;
	CREATE_THREAD(mThread, run, this);
	LOG_INFO("�߳������ɹ� : %s", mName.c_str());
}

void CustomThread::stop()
{
	LOG_INFO("׼���˳��߳� : %s", mName.c_str());
	mRunning = false;
	mPause = false;
	while (!mIsBackground && !mFinish) {}
	CLOSE_THREAD(mThread);
	mCallback = NULL;
	TRACE_DELETE(mTimeLock);
	LOG_INFO("�߳��˳����! �߳��� : %s", mName.c_str());
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
			LOG_INFO("�����߳��쳣! �߳��� : %s", mName.c_str());
		}
	}
	mFinish = true;
}