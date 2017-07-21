﻿#ifdef _TRACE_MEMERY

#include "txMemeryTrace.h"
#include "txLog.h"
#ifdef _WRITE_FILE
#include "txSerializer.h"
#endif

txMap<void*, MemeryInfo> MemeryTrace::mMemeryInfo;
txMap<std::string, MemeryType> MemeryTrace::mMemeryType;
txSet<std::string> MemeryTrace::mIgnoreClass;
txSet<std::string> MemeryTrace::mIgnoreClassKeyword;
txSet<std::string> MemeryTrace::mShowOnlyDetailClass;
txSet<std::string> MemeryTrace::mShowOnlyStatisticsClass;
bool MemeryTrace::mShowDetail = true;
bool MemeryTrace::mShowStatistics = true;
bool MemeryTrace::mShowTotalCount = true;
int MemeryTrace::mInstanceCount = 0;
bool MemeryTrace::mShowAll = true;
#ifdef _WRITE_FILE
txMap<std::string, int> MemeryTrace::mMemeryTypeIndex;
MemeryType MemeryTrace::mMemeryList[MAX_COUNT];
int MemeryTrace::mMemeryCount = 0;
#endif
CFFMServer* MemeryTrace::mServer = NULL;
bool MemeryTrace::mLockWriteInfo = false;
bool MemeryTrace::mLockReadInfo = false;

MemeryTrace::MemeryTrace()
{
	++mInstanceCount;
	assert(mInstanceCount <= 1);
	mShowDetail = true;
	mShowStatistics = true;
	mShowTotalCount = true;
	mShowAll = true;
	mServer = TRACE_NEW(CFFMServer, mServer);
	mServer->Create();
	mThread = NULL;
}

MemeryTrace::~MemeryTrace()
{
	if (mThread != NULL)
	{
		TerminateThread(mThread, 0);
		mThread = NULL;
	}
	Sleep(100);
	TRACE_DELETE(mServer);
}

DWORD WINAPI MemeryTrace::debugMemeryTrace(LPVOID lpParameter)
{
	while (true)
	{
		static DWORD beginTime = timeGetTime();
		DWORD curTime = timeGetTime();
		if (curTime - beginTime < 1000)
		{
			Sleep(1000 - (curTime - beginTime));
		}
		curTime = timeGetTime();
		int memCount = mMemeryInfo.size();
		int memSize = 0;
		if (mShowAll)
		{
			// 首先检测是否可以读取,如果不可以,则等待解锁读取
			waitUnlockRead();
			// 正在对列表进行读取操作,所以要锁定列表的写入
			lockWriteInfo();
			LOGI("\n\n---------------------------------------------memery info begin-----------------------------------------------------------\n");

			// 内存详细信息
			txMap<void*, MemeryInfo>::iterator iter = mMemeryInfo.begin();
			txMap<void*, MemeryInfo>::iterator iterEnd = mMemeryInfo.end();
			FOR_STL (mMemeryInfo, ; iter != iterEnd; ++iter)
			{
				memSize += iter->second.size;
				if (!mShowDetail)
				{
					continue;
				}
				// 如果该类型已忽略,则不显示
				if (mIgnoreClass.find(iter->second.type) != mIgnoreClass.end())
				{
					continue;
				}

				// 如果仅显示的类型列表不为空,则只显示列表中的类型
				if (mShowOnlyDetailClass.size() > 0 && mShowOnlyDetailClass.find(iter->second.type) == mShowOnlyDetailClass.end())
				{
					continue;
				}

				// 如果类型包含关键字,则不显示
				bool show = true;
				txSet<std::string>::iterator iterKeyword = mIgnoreClassKeyword.begin();
				txSet<std::string>::iterator iterKeywordEnd = mIgnoreClassKeyword.end();
				FOR_STL (mIgnoreClassKeyword, ; iterKeyword != iterKeywordEnd; ++iterKeyword)
				{
					if (strstr(iter->second.type.c_str(), iterKeyword->c_str()) != NULL)
					{
						show = false;
						break;
					}
				}
				END_FOR_STL(mIgnoreClassKeyword);

				if (show)
				{
					LOGI("size : %d, file : %s, line : %d, class : %s\n", iter->second.size, iter->second.file.c_str(), iter->second.line, iter->second.type.c_str());
				}
			}
			END_FOR_STL(mMemeryInfo);

			// 解锁列表的写入
			unlockWriteInfo();

			if (mShowTotalCount)
			{
				LOGI("-------------------------------------------------memery count : %d, total size : %.3fKB\n", memCount, memSize / 1000.0f);
			}

			if (mShowStatistics)
			{
				txMap<std::string, MemeryType >::iterator iterType = mMemeryType.begin();
				txMap<std::string, MemeryType >::iterator iterTypeEnd = mMemeryType.end();
				FOR_STL (mMemeryType, ; iterType != iterTypeEnd; ++iterType)
				{
					// 如果该类型已忽略,则不显示
					if (mIgnoreClass.find(iterType->first) != mIgnoreClass.end())
					{
						continue;
					}
					// 如果仅显示的类型列表不为空,则只显示列表中的类型
					if (mShowOnlyStatisticsClass.size() > 0 && mShowOnlyStatisticsClass.find(iterType->first) == mShowOnlyStatisticsClass.end())
					{
						continue;
					}
					// 如果类型包含关键字,则不显示
					bool show = true;
					txSet<std::string>::iterator iterKeyword = mIgnoreClassKeyword.begin();
					txSet<std::string>::iterator iterKeywordEnd = mIgnoreClassKeyword.end();
					FOR_STL (mIgnoreClassKeyword, ; iterKeyword != iterKeywordEnd; ++iterKeyword)
					{
						if (strstr(iterType->first.c_str(), iterKeyword->c_str()) != NULL)
						{
							show = false;
							break;
						}
					}
					END_FOR_STL(mIgnoreClassKeyword);
					if (show)
					{
						LOGI("%s : %d个, %.3fKB\n", iterType->first.c_str(), iterType->second.count, iterType->second.size / 1000.0f);
					}
				}
				END_FOR_STL(mMemeryType);
			}
			LOGI("---------------------------------------------memery info end-----------------------------------------------------------\n");
		}
		beginTime = curTime;
	}
	return 0;
}

#ifdef _WRITE_FILE
DWORD WINAPI MemeryTrace::writeMemeryTrace(LPVOID lpParameter)
{
	while (true)
	{
		static DWORD beginTime = timeGetTime();
		DWORD curTime = timeGetTime();
		if (curTime - beginTime < 1000)
		{
			Sleep(1000 - (curTime - beginTime));
		}
		curTime = timeGetTime();
		// 这里就不能在对序列化的内存进行跟踪,否则会陷入死锁
		txSerializer serializer(false);

		// 首先检测是否可以读取,如果不可以,则等待解锁读取
		waitUnlockRead();
		// 正在对列表进行读取操作,所以要锁定列表的写入
		lockWriteInfo();
		// 写入详细信息数量
		int infoCount = mMemeryInfo.size();
		serializer.write(infoCount);
		txMap<void*, MemeryInfo>::iterator iterInfo = mMemeryInfo.begin();
		txMap<void*, MemeryInfo>::iterator iterInfoEnd = mMemeryInfo.end();
		FOR_STL (mMemeryInfo, ; iterInfo != iterInfoEnd; ++iterInfo)
		{
			// 写入地址
			serializer.write(iterInfo->first);
			// 写入内存大小
			serializer.write(iterInfo->second.size);
			// 写入文件名
			serializer.writeString(iterInfo->second.file.c_str());
			// 写入行号
			serializer.write(iterInfo->second.line);
			// 写入类型
			serializer.writeString(iterInfo->second.type.c_str());
		}
		END_FOR_STL(mMemeryInfo);

		// 写入类型数量
		int typeCount = mMemeryTypeIndex.size();
		serializer.write(typeCount);
		txMap<std::string, int>::iterator iterIndex = mMemeryTypeIndex.begin();
		txMap<std::string, int>::iterator iterIndexEnd = mMemeryTypeIndex.end();
		FOR_STL (mMemeryTypeIndex, ; iterIndex != iterIndexEnd; ++iterIndex)
		{
			// 写入类型名
			serializer.writeString(mMemeryList[iterIndex->second].type.c_str());
			// 写入个数
			serializer.write(mMemeryList[iterIndex->second].count);
			// 写入大小
			serializer.write(mMemeryList[iterIndex->second].size);
		}
		END_FOR_STL(mMemeryTypeIndex);

		// 解锁列表的写入
		unlockWriteInfo();

		mServer->WriteCmdData(1, serializer.getDataSize(), (void*)serializer.getBuffer());
		
		beginTime = curTime;
	}
	return 0;
}
#endif

#endif