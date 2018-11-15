#include "TimeLock.h"
#include "Utility.h"

TimeLock::TimeLock(long frameTimeMS, long forceSleep)
{
	mFrameTimeMS = frameTimeMS;
	mForceSleep = forceSleep;
	mLastTime = getTimeMS();
	mCurTime = mLastTime;
}

long TimeLock::update()
{
	long endTime = getTimeMS();
	long remainMS = mFrameTimeMS - (endTime - mCurTime);
	if (remainMS > 0)
	{
		sleep(remainMS);
	}
	else if (mForceSleep > 0)
	{
		sleep(mForceSleep);
	}
	mCurTime = getTimeMS();
	long frameTime = mCurTime - mLastTime;
	mLastTime = mCurTime;
	return frameTime;
}