#include "txDataManager.h"
#include "txUtility.h"
#include "GameLog.h"

txDataManager::txDataManager()
{
	throwAllToUnusedList();
}

txDataManager::~txDataManager()
{
	dump();
	DestroyAll();
}

int txDataManager::LoadData(const char *name, const bool& neednull)
{
	// 加载文件时锁定文件
	LOCK(mDataLock, LT_READ);
	std::map<std::string, int>::iterator iterData = mDataNameList.find(txUtility::getFileName(name));
	if (iterData != mDataNameList.end())
	{
		UNLOCK(mDataLock, LT_READ);
		return iterData->second;
	}
	int index = -1;
	if(!getOneUnusedIndex(index))
	{
		UNLOCK(mDataLock, LT_READ);
		GAME_ERROR("error : can not get one unused index! name is : %s", name);
		return -1;
	}

#ifdef LOAD_FROM_ASSETMANAGER
	int nFileLen = ASS_getFileSize(const_cast<char*>(name));
	if(nFileLen <= 0)
	{
		UNLOCK(mDataLock, LT_READ);
		ENGINE_ERROR("error : can not open file in AssetManager, path is : %s", name);
		return -1;
	}
	if(mShowDebugInfo)
	{
		LOGI("file length is %d, name is : %s", nFileLen, name);
	}

	elems[index].setSize(nFileLen + neednull);
	elems[index].setKey(name);
	mDataList.insert(std::make_pair(name, index));
	throwIndexToUsedList(index);

	char* data = elems[index].getValuePtr();
	memset(data, 0, elems[index].getSize());
	ASS_loadFile((char*)name, nFileLen, data);
#else
	std::string path = txUtility::getAvailableResourcePath(std::string(name));
	if (mShowDebugInfo)
	{
		LOG_INFO("Load data from %s", path.c_str());
	}

	//打开文件，开始载入
#ifdef _USE_SAFE_API
	FILE* fp = NULL;
	fopen_s(&fp, path.c_str(), "rb");
#else
	FILE* fp = fopen(path.c_str(), "rb");
#endif
	if (fp == NULL)
	{
		UNLOCK(mDataLock, LT_READ);
		// 此处不能使用错误信息宏,因为需要允许某些文件不存在的情况
		LOG_INFO("can not open file in disk, path is : %s", path.c_str());
		return -1;
	}

	//获取文件大小
	fseek(fp, 0, SEEK_END);
	int nFileLen = ftell(fp);
	rewind(fp);

	//空文件
	if (nFileLen <= 0)
	{
		// 此处不能使用错误信息宏,因为需要允许某些文件为空的情况
		LOG_INFO("file len is 0, path is : %s", path.c_str());
	}

	elems[index].setSize(nFileLen + neednull);
	elems[index].setKey(name);
	mDataNameList.insert(std::make_pair(txUtility::getFileName(name), index));
	throwIndexToUsedList(index);

	char* data = elems[index].getValuePtr();
	memset(data, 0, elems[index].getSize());
	int count = fread(data, 1, nFileLen, fp);
	if (count != nFileLen)
	{
		GAME_ERROR("error : fread count != nFileLen, count : %d, nFileLen : %d, path : %s", count, nFileLen, path.c_str());
	}
	fclose(fp);
#endif
	UNLOCK(mDataLock, LT_READ);
	return index;
}

int txDataManager::NewData(const char *name, const int& sizeInByte)
{
	// 创建文件时锁定文件
	LOCK(mDataLock, LT_WRITE);
	int index = -1;
	if(!getOneUnusedIndex(index))
	{
		UNLOCK(mDataLock, LT_WRITE);
		GAME_ERROR("error : can not find one unused index, in NewData, name is : %s", name);
		return -1;
	}

	elems[index].setSize(sizeInByte);
	elems[index].setKey(name);
	mDataNameList.insert(std::make_pair(txUtility::getFileName(name), index));
	throwIndexToUsedList(index);
	UNLOCK(mDataLock, LT_WRITE);
	return index;
}

bool txDataManager::DestroyData(const int& index)
{
	// 销毁文件时锁定文件
	LOCK(mDataLock, LT_WRITE);
	bool ret = throwIndexToUnusedList(index);
	UNLOCK(mDataLock, LT_WRITE);
	return ret;
}

void txDataManager::DestroyAll()
{
	// 销毁全部文件时锁定文件
	LOCK(mDataLock, LT_WRITE);
	throwAllToUnusedList();
	for (int i = 0; i < DATA_SLOTS; ++i)
	{
		elems[i].setSize(0);
		elems[i].setKey("");
	}
	mDataNameList.clear();
	UNLOCK(mDataLock, LT_WRITE);
}

int txDataManager::GetDataIndex(char* name) const
{
	for (int i = 0; i < DATA_SLOTS; ++i)
	{
		if (isIndexUsed(i) && (strcmp(name, elems[i].getKeyPtr()) == 0))
		{
			return i;
		}
	}
	GAME_ERROR("error : can not find file named : %s", name);
	return -1;
}

txDataElem* txDataManager::GetData(const int& index) const
{
	if (isIndexUsed(index))
	{
		const txDataElem* dataPtr = &(elems[index]);
		return (txDataElem*)dataPtr;
	}
	else
	{
		GAME_ERROR("error : can not get file, index is : %d", index);
		return NULL;
	}
}

void txDataManager::dump() const
{
	if (mShowDebugInfo)
	{
		LOG_INFO("======================================Begin Data Dump====================================");
		std::set<short>::iterator iter = mUsedIndexList.begin();
		std::set<short>::iterator iterEnd = mUsedIndexList.end();
		for (; iter != iterEnd; ++iter)
		{
			const int& index = *iter;
			LOG_INFO("========%08d, k=%s, size=%d", index, elems[index].getKeyPtr(), elems[index].getSize());
		}
		LOG_INFO("========                Total inuse :%d", (int)mUsedIndexList.size());
		LOG_INFO("======================================End Data Dump======================================");
	}
}

int txDataManager::GetFileSize(const int& index) const
{
#ifdef LOAD_FROM_ASSETMANAGER
	std::set<short>::iterator iter = mUsedIndexList.find(index);
	if (iter != mUsedIndexList.end())
	{
		return ASS_getFileSize(elems[*iter].getKeyPtr());
	}
	else
	{
		ENGINE_ERROR("error : can not get file size, index is : %d", index);
		return 0;
	}
#else
	if (isIndexUsed(index))
	{
		return elems[index].getSize();
	}
	else
	{
		GAME_ERROR("error : can not get file size, index is : %d", index);
		return 0;
	}
#endif
}

int txDataManager::GetFileSize(const char* filename, int* pStart, int* pLength, int* pFD) const
{
#ifdef LOAD_FROM_ASSETMANAGER
	return ASS_getFileSize((char*)filename, pStart, pLength, pFD);
#else
	struct stat info;
	std::string path = txUtility::getAvailableResourcePath(filename);
	stat(path.c_str(), &info);
	return info.st_size;
#endif
}

bool txDataManager::isFileExist(const std::string& fileName) const
{
	std::string path = txUtility::getAvailableResourcePath(fileName);
	return txUtility::isFileExist(path);
}

// 获取一个未使用的下标
bool txDataManager::getOneUnusedIndex(int& index)
{
	if(mUnusedIndexList.size() == 0)
	{
		GAME_ERROR("error : there is no more space for data! please expand the reserved memory! txData::getOneUnusedIndex");
		return false;
	}
	index = *mUnusedIndexList.begin();
	return true;
}

// 将未使用列表中的一个下标放入已使用列表中,并且从未使用列表中删除该下标
bool txDataManager::throwIndexToUsedList(const int& index)
{
	std::set<short>::iterator iter = mUnusedIndexList.find(index);
	if(iter == mUnusedIndexList.end())
	{
		GAME_ERROR("error : can not find index : %d, the index is not in unusedlist.", index);
		return false;
	}
	mUnusedIndexList.erase(iter);
	mUsedIndexList.insert(index);
	return true;
}

// 将已使用列表中的一个下标放入未使用列表中,将该下标的数据销毁并且从已使用列表中删除该下标
bool txDataManager::throwIndexToUnusedList(const int& index)
{
	std::set<short>::iterator iter = mUsedIndexList.find(index);
	if(iter == mUsedIndexList.end())
	{
		GAME_ERROR("error : can not find index : %d, the index is not in usedlist.", index);
		return false;
	}
	mUsedIndexList.erase(iter);

	if (mShowDebugInfo)
	{
		LOG_INFO("destroy data : %s, size : %d", elems[index].getKeyPtr(), elems[index].getSize());
	}
	std::map<std::string, int>::iterator iterData = mDataNameList.find(txUtility::getFileName(elems[index].getKeyPtr()));
	if (iterData != mDataNameList.end())
	{
		mDataNameList.erase(iterData);
	}
	elems[index].setSize(0);
	elems[index].setKey("");
	mUnusedIndexList.insert(index);
	return true;
}

// 将全部下标都放入未使用列表中,并且清空已使用列表
void txDataManager::throwAllToUnusedList()
{
	// 清空已使用列表
	mUsedIndexList.clear();
	// 将所有索引放入未使用列表中
	mUnusedIndexList.clear();
	for(int i = 0; i < DATA_SLOTS; ++i)
	{
		mUnusedIndexList.insert(i);
	}
}
