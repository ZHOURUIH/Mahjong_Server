#include "txSTLBase.h"
#include "ServerDefine.h"
#include "MemoryDefine.h"

txSTLBase::txSTLBase()
{
	mLock = SL_NONE;
	mReadLockCount = 0;
	mLine = 0;
}
txSTLBase::~txSTLBase()
{
	;
}
// 循环遍历列表之前必须锁定
void txSTLBase::lock(STL_LOCK lockType, const string& file, int line)
{
	// 当前为锁定写入,则不允许再加任何锁,或者当前为读锁定,则不能添加写锁定
	if (mLock == SL_WRITE || (mLock == SL_READ && lockType != SL_READ) || lockType == SL_NONE)
	{
		directError("can not lock stl before unlock!");
		return;
	}
	mFile = file;
	mLine = line;
	mLock = lockType;
	if (mLock == SL_READ)
	{
		// 读锁定计数加1
		++mReadLockCount;
	}
}
// 循环遍历列表结束以后必须解锁
void txSTLBase::unlock(STL_LOCK lockType)
{
	// 锁定类型不一致,则不能解锁
	if (mLock == SL_NONE || mLock != lockType)
	{
		directError("can not unlock stl before lock!");
		return;
	}
	if (mLock == SL_READ)
	{
		// 读锁定计数减1,当计数为0时解锁
		--mReadLockCount;
		if (mReadLockCount == 0)
		{
			mLock = SL_NONE;
		}
	}
	else if (mLock == SL_WRITE)
	{
		mLock = SL_NONE;
	}
}
void txSTLBase::checkLock()
{
	if (mLock != SL_NONE)
	{
		directError("error : stl is locked! can not modify stl!");
	}
}
void txSTLBase::directError(const string& info)
{
	LOG_ERROR(info);
}

// 重置锁
void txSTLBase::resetLock()
{
	mLock = SL_NONE;
	mReadLockCount = 0;
	mFile = "";
	mLine = 0;
}