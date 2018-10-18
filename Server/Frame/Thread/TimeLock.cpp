#include "TimeLock.h"
#include "Utility.h"

TimeLock::TimeLock(long frameTimeMS, long forceSleep)
{
	mFrameTimeMS = frameTimeMS;
	mForceSleep = forceSleep;
	mLastTime = SystemUtility::getTimeMS();
	mCurTime = mLastTime;
}

long TimeLock::update()
{
	long endTime = SystemUtility::getTimeMS();
	long remainMS = mFrameTimeMS - (endTime - mCurTime);
	if (remainMS > 0)
	{
		SystemUtility::sleep(remainMS);
	}
	else if (mForceSleep > 0)
	{
		SystemUtility::sleep(mForceSleep);
	}
	mCurTime = SystemUtility::getTimeMS();
	long frameTime = mCurTime - mLastTime;
	mLastTime = mCurTime;
	return frameTime;
}