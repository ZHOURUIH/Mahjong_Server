#ifndef _TX_STL_BASE_H_
#define _TX_STL_BASE_H_

#include <string>
#include <vector>

using namespace std;

enum STL_LOCK
{
	SL_NONE,
	SL_READ,
	SL_WRITE,
};

class ThreadLock;
class txSTLBase
{
public:
	txSTLBase();
	virtual ~txSTLBase();
	// 循环遍历列表之前必须锁定
	void lock(STL_LOCK lockType, const string& file, int line);
	// 循环遍历列表结束以后必须解锁
	void unlock(STL_LOCK lockType);
protected:
	void checkLock();
	void directError(const string& info);
	void resetLock();
protected:
	STL_LOCK mLock;
	int mReadLockCount;		// 读锁定的次数,读锁定可以叠加,计算读锁定的次数,当读锁定次数为0时,取消锁定
	string mFile;
	int mLine;
};

#endif