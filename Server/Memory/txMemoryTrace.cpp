#include "txMemoryTrace.h"
#ifdef TRACE_MEMORY
#include "txSerializer.h"
#include "TimeLock.h"
#include "CustomThread.h"

txMap<void*, MemoryInfo> txMemoryTrace::mMemoryInfo;
txMap<std::string, MemoryType> txMemoryTrace::mMemoryType;
txSet<std::string> txMemoryTrace::mIgnoreClass;
txSet<std::string> txMemoryTrace::mIgnoreClassKeyword;
txSet<std::string> txMemoryTrace::mShowOnlyDetailClass;
txSet<std::string> txMemoryTrace::mShowOnlyStatisticsClass;
bool txMemoryTrace::mShowDetail = true;
bool txMemoryTrace::mShowStatistics = true;
bool txMemoryTrace::mShowTotalCount = true;
bool txMemoryTrace::mShowAll = true;
txMap<std::string, int> txMemoryTrace::mMemoryTypeIndex;
MemoryType txMemoryTrace::mMemoryList[MAX_COUNT];
int txMemoryTrace::mMemoryCount = 0;
txShareMemoryServer* txMemoryTrace::mShareMemoryServer = NULL;
ThreadLock txMemoryTrace::mInfoLock;
bool txMemoryTrace::mWriteOrDebug = false;
CustomThread* txMemoryTrace::mThread = NULL;

txMemoryTrace::txMemoryTrace()
{
	mShowDetail = true;
	mShowStatistics = true;
	mShowTotalCount = true;
	mShowAll = true;
	mShareMemoryServer = TRACE_NEW(txShareMemoryServer, mShareMemoryServer);
	mThread = TRACE_NEW(CustomThread, mThread, "MemoryTrace");
}

txMemoryTrace::~txMemoryTrace()
{
	TRACE_DELETE(mThread);
	TRACE_DELETE(mShareMemoryServer);
}

void txMemoryTrace::init(const bool& writeOrDebug)
{
	mShareMemoryServer->Create();
	mWriteOrDebug = writeOrDebug;
	mThread->start(mWriteOrDebug ? writeMemoryTrace : debugMemoryTrace, NULL, 1000);
}

bool txMemoryTrace::debugMemoryTrace(void* args)
{
	int memoryCount = mMemoryInfo.size();
	int memorySize = 0;
	if (mShowAll)
	{
		// ���ȼ���Ƿ���Զ�ȡ,���������,��ȴ�������ȡ
		LOCK(mInfoLock);
		LOG_INFO("\n\n---------------------------------------------memery info begin-----------------------------------------------------------\n");

		// �ڴ���ϸ��Ϣ
		auto iter = mMemoryInfo.begin();
		auto iterEnd = mMemoryInfo.end();
		FOR_STL(mMemoryInfo, ; iter != iterEnd; ++iter)
		{
			memorySize += iter->second.size;
			if (!mShowDetail)
			{
				continue;
			}
			// ����������Ѻ���,����ʾ
			if (mIgnoreClass.find(iter->second.type) != mIgnoreClass.end())
			{
				continue;
			}

			// �������ʾ�������б�Ϊ��,��ֻ��ʾ�б��е�����
			if (mShowOnlyDetailClass.size() > 0 && mShowOnlyDetailClass.find(iter->second.type) == mShowOnlyDetailClass.end())
			{
				continue;
			}

			// ������Ͱ����ؼ���,����ʾ
			bool show = true;
			auto iterKeyword = mIgnoreClassKeyword.begin();
			auto iterKeywordEnd = mIgnoreClassKeyword.end();
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
				LOG_INFO("size : %d, file : %s, line : %d, class : %s\n", iter->second.size, iter->second.file.c_str(), iter->second.line, iter->second.type.c_str());
			}
		}
		END_FOR_STL(mMemoryInfo);
		UNLOCK(mInfoLock);

		if (mShowTotalCount)
		{
			LOG_INFO("-------------------------------------------------memery count : %d, total size : %.3fKB\n", memoryCount, memorySize / 1000.0f);
		}
		// ��ʾͳ������
		if (mShowStatistics)
		{
			auto iterType = mMemoryType.begin();
			auto iterTypeEnd = mMemoryType.end();
			FOR_STL (mMemoryType, ; iterType != iterTypeEnd; ++iterType)
			{
				// ����������Ѻ���,����ʾ
				if (mIgnoreClass.find(iterType->first) != mIgnoreClass.end())
				{
					continue;
				}
				// �������ʾ�������б�Ϊ��,��ֻ��ʾ�б��е�����
				if (mShowOnlyStatisticsClass.size() > 0 && mShowOnlyStatisticsClass.find(iterType->first) == mShowOnlyStatisticsClass.end())
				{
					continue;
				}
				// ������Ͱ����ؼ���,����ʾ
				bool show = true;
				auto iterKeyword = mIgnoreClassKeyword.begin();
				auto iterKeywordEnd = mIgnoreClassKeyword.end();
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
					LOG_INFO("%s : %d��, %.3fKB\n", iterType->first.c_str(), iterType->second.count, iterType->second.size / 1000.0f);
				}
			}
			END_FOR_STL(mMemoryType);
		}
		LOG_INFO("---------------------------------------------memery info end-----------------------------------------------------------\n");
	}
	return true;
}

bool txMemoryTrace::writeMemoryTrace(void* args)
{
	// ����Ͳ����ڶ����л����ڴ���и���,�������������
	txSerializer serializer(false);
	// �����б�
	LOCK(mInfoLock);
	// д����ϸ��Ϣ����
	int infoCount = mMemoryInfo.size();
	serializer.write(infoCount);
	auto iterInfo = mMemoryInfo.begin();
	auto iterInfoEnd = mMemoryInfo.end();
	FOR_STL(mMemoryInfo, ; iterInfo != iterInfoEnd; ++iterInfo)
	{
		serializer.write(iterInfo->first);						// ��ַ
		serializer.write(iterInfo->second.size);				// �ڴ��С
		serializer.writeString(iterInfo->second.file.c_str());	// �ļ���
		serializer.write(iterInfo->second.line);				// �к�
		serializer.writeString(iterInfo->second.type.c_str());	// ����
	}
	END_FOR_STL(mMemoryInfo);

	// д����������
	int typeCount = mMemoryTypeIndex.size();
	serializer.write(typeCount);
	auto iterIndex = mMemoryTypeIndex.begin();
	auto iterIndexEnd = mMemoryTypeIndex.end();
	FOR_STL (mMemoryTypeIndex, ; iterIndex != iterIndexEnd; ++iterIndex)
	{
		serializer.writeString(mMemoryList[iterIndex->second].type.c_str());	// ������
		serializer.write(mMemoryList[iterIndex->second].count);					// ����
		serializer.write(mMemoryList[iterIndex->second].size);					// ��С
	}
	END_FOR_STL(mMemoryTypeIndex);
	// �����б�
	UNLOCK(mInfoLock);
	mShareMemoryServer->WriteCmdData(1, serializer.getDataSize(), (void*)serializer.getBuffer());
	return true;
}

void txMemoryTrace::insertPtr(void* ptr, MemoryInfo& info)
{
	// �����б�
	LOCK(mInfoLock);
	int lastPos = info.file.find_last_of('\\');
	if (lastPos != -1)
	{
		info.file = info.file.substr(lastPos + 1, info.file.length() - lastPos - 1);
	}
	mMemoryInfo.insert(ptr, info);

	auto iterType = mMemoryType.find(info.type);
	if (iterType != mMemoryType.end())
	{
		++(iterType->second.count);
		iterType->second.size += info.size;
	}
	else
	{
		mMemoryType.insert(info.type, MemoryType(info.type, 1, info.size));
	}

	if(mWriteOrDebug)
	{
		// �������±��б��в��Ҹ�����,�����,�����������Ϣ
		auto iterIndex = mMemoryTypeIndex.find(info.type);
		if (iterIndex != mMemoryTypeIndex.end())
		{
			auto iterType = mMemoryType.find(info.type);
			mMemoryList[iterIndex->second] = iterType->second;
		}
		// ���û��,�����������Ϣ
		else
		{
			if (mMemoryCount < MAX_COUNT)
			{
				auto iterType = mMemoryType.find(info.type);
				mMemoryTypeIndex.insert(info.type, mMemoryCount);
				mMemoryList[mMemoryCount] = iterType->second;
				++mMemoryCount;
			}
			else
			{
				UNLOCK(mInfoLock);
				return;
			}
		}
	}
	
	// �����б�
	UNLOCK(mInfoLock);
}

void txMemoryTrace::erasePtr(void* ptr)
{
	// �����б�
	LOCK(mInfoLock);
	// ��һ��ѭ����Ϊ�˷������
	do
	{
		// ���ڴ���Ϣ�б����Ƴ�
		auto iterTrace = mMemoryInfo.find(ptr);
		if (iterTrace == mMemoryInfo.end())
		{
			break;
		}
		std::string type = iterTrace->second.type;
		int size = iterTrace->second.size;
		mMemoryInfo.erase(iterTrace);
		// ���ڴ������б����Ƴ�
		auto iterType = mMemoryType.find(type);
		if (iterType == mMemoryType.end())
		{
			break;
		}
		--(iterType->second.count);
		iterType->second.size -= size;
		if(mWriteOrDebug)
		{
			// ���±��б��в��Ҹ����͵��±�,�����,��������Ϣ�е���Ϣ���
			auto iterIndex = mMemoryTypeIndex.find(type);
			if (iterIndex == mMemoryTypeIndex.end())
			{
				break;
			}
			--(mMemoryList[iterIndex->second].count);
			mMemoryList[iterIndex->second].size -= size;
		}
	} while (false);
	// �����б�
	UNLOCK(mInfoLock);
}

#endif