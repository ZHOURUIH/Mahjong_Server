#include "txCommand.h"
#include "CommandReceiver.h"

void txCommand::init()
{
	mReceiver = NULL;
	mShowDebugInfo = true;
	mDelayCommand = false;
	mValid = false;
	mAssignID = -1;
	mResult = NULL;
	mExecuteState = ES_NOT_EXECUTE;
	mFile = EMPTY_STRING;
	mLine = 0;
	mEndCallback.clear();
	mStartCallback.clear();
	reset();
}

void txCommand::runEndCallBack()
{
	int callbackCount = mEndCallback.size();
	for (int i = 0; i < callbackCount; ++i)
	{
		if (mEndCallback[i].first != NULL)
		{
			mEndCallback[i].first(this, mEndCallback[i].second);
		}
	}
	mEndCallback.clear();
}
void txCommand::runStartCallBack()
{
	int callbackCount = mStartCallback.size();
	for (int i = 0; i < callbackCount; ++i)
	{
		if (mStartCallback[i].first != NULL)
		{
			mStartCallback[i].first(this, mStartCallback[i].second);
		}
	}
	mStartCallback.clear();
}