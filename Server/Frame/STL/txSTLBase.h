﻿#ifndef _TX_STL_BASE_H_
#define _TX_STL_BASE_H_

class txSTLBase
{
public:
	txSTLBase()
	{
		mLock = false;
		mLine = 0;
	}
	virtual ~txSTLBase(){}
	bool isLocked() { return mLock; }
	// 循环遍历列表之前必须锁定
	virtual void lock(const std::string& file, int line) 
	{
		if (mLock)
		{
			static char temp[512];
			SPRINTF(temp, 512, "error : can not lock stl before unlock! curlock file : %s, line : %d, lastlock file : %s, line : %d", file.c_str(), line, mFile.c_str(), mLine);
			directError(temp);
		}
		mFile = file;
		mLine = line;
		mLock = true; 
	}
	// 循环遍历列表结束以后必须解锁
	virtual void unlock() 
	{
		if (!mLock)
		{
			static char temp[512];
			SPRINTF(temp, 512, "error : can not unlock stl before lock! file : %s, line : %d", mFile.c_str(), mLine);
			directError(temp);
		}
		mLock = false; 
	}
protected:
	void checkLock()
	{
		if (mLock)
		{
			directError("error : stl is locked! can not modify stl!");
		}
	}
	void directError(const std::string& info)
	{
		LOG_ERROR("%s", info.c_str());
	}
protected:
	bool mLock;
	std::string mFile;
	int mLine;
};

#endif