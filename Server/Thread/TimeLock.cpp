#include "TimeLock.h"
#include "txUtility.h"

TimeLock::TimeLock(const long& frameTimeMS)
{
	mFrameTimeMS = frameTimeMS;
	mLastTime = 0;
	mForceSleep = 0;
	mLastTime = txUtility::getTimeMS();
}

long TimeLock::update()
{
	long curTime = txUtility::getTimeMS();
	long thisFrameTime = curTime - mLastTime;
	long remainMS = mFrameTimeMS - thisFrameTime;
	if (remainMS > 0)
	{
		txUtility::sleep(remainMS);
	}
	else if (mForceSleep > 0)
	{
		txUtility::sleep(mForceSleep);
	}
	mLastTime = curTime;
	return thisFrameTime;
}