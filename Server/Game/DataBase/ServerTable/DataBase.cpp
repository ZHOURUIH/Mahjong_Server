#include "DataBase.h"
#include "DataFactory.h"
#include "Utility.h"

txMap<DATA_TYPE, DataFactoryBase*> DataBase::mDataFactoryList;
txMap<std::string, DATA_TYPE> DataBase::mDataFileDefine;
txMap<DATA_TYPE, std::string> DataBase::mDataDefineFile;

void DataBase::init()
{
	initDataFactory();
	loadAllDataFromFile();
}

void DataBase::addDataFactoryToList(DataFactoryBase* factory)
{
	mDataFactoryList.insert(factory->getType(), factory);
}

void DataBase::destroyDataFactory()
{
	auto iterDataFactoryList = mDataFactoryList.begin();
	auto iterDataFactoryListEnd = mDataFactoryList.end();
	FOR(mDataFactoryList, ; iterDataFactoryList != iterDataFactoryListEnd; ++iterDataFactoryList)
	{
		TRACE_DELETE(iterDataFactoryList->second);
	}
	END(mDataFactoryList);
	mDataFactoryList.clear();
}

bool DataBase::addData(DATA_TYPE type, Data* data, int pos)
{
	if (data == NULL)
	{
		return false;
	}
	if (data->getType() != type)
	{
		return false;
	}
	auto iter = mDataStructList.find(type);
	if (iter != mDataStructList.end())
	{
		if (pos == -1)
		{
			iter->second.push_back(data);
		}
		else if (pos >= 0 && pos <= (int)iter->second.size())
		{
			iter->second.insert(iter->second.begin() + pos, data);
		}
	}
	else
	{
		txVector<Data*> dataList;
		dataList.push_back(data);
		mDataStructList.insert(type, dataList);
	}
	return true;
}

bool DataBase::deleteData(DATA_TYPE type, int index)
{
	auto iter = mDataStructList.find(type);
	if (iter != mDataStructList.end())
	{
		if (index >= (int)iter->second.size())
		{
			return false;
		}
		auto dataFactory = getDataFactory(type);
		if (dataFactory == NULL)
		{
			return false;
		}
		dataFactory->destroyData(iter->second[index]);
		iter->second.erase(iter->second.begin() + index);
		return true;
	}
	return false;
}

Data* DataBase::createData(DATA_TYPE type)
{
	auto factory = getDataFactory(type);
	if (factory != NULL)
	{
		return factory->createData();
	}
	return NULL;
}

void DataBase::loadAllDataFromFile()
{
	// 读取配置文件，获得需要加载的所有数据列表
	auto iter = mDataFactoryList.begin();
	auto iterEnd = mDataFactoryList.end();
	FOR(mDataFactoryList, ; iter != iterEnd; ++iter)
	{
		const std::string& fileName = mDataDefineFile.tryGet(iter->first, EMPTY_STRING);
		loadData(SystemUtility::getAvailableResourcePath(GAME_DATA_PATH) + fileName + ".table", true);
	}
	END(mDataFactoryList);
}

void  DataBase::destroyAllData()
{
	auto iterStructList = mDataStructList.begin();
	auto iterStructListEnd = mDataStructList.end();
	FOR(mDataStructList, ; iterStructList != iterStructListEnd; ++iterStructList)
	{
		auto factory = getDataFactory(iterStructList->first);
		if (factory != NULL)
		{
			int dataCount = iterStructList->second.size();
			for (int i = 0; i < dataCount; ++i)
			{
				factory->destroyData((iterStructList->second)[i]);
			}
			iterStructList->second.clear();
		}
		else
		{
			LOG_ERROR("error : can not find data factory : %d, DataBase::destroyAllData", (int)iterStructList->first);
		}
	}
	END(mDataStructList);
	mDataStructList.clear();
}

void DataBase::destroyData(DATA_TYPE type)
{
	auto iterStructList = mDataStructList.find(type);
	if (iterStructList != mDataStructList.end())
	{
		auto factory = getDataFactory(iterStructList->first);
		if (factory != NULL)
		{
			int dataCount = iterStructList->second.size();
			FOR(iterStructList->second, int i = 0; i < dataCount; ++i)
			{
				factory->destroyData((iterStructList->second)[i]);
			}
			END(iterStructList->second);
			iterStructList->second.clear();
		}
		mDataStructList.erase(iterStructList);
	}
}

void DataBase::loadData(const std::string& filePath, bool forceCover)
{
	// 根据文件名查找工厂类型
	std::string fileName = StringUtility::getFileNameNoSuffix(filePath);
	auto fileDefine = mDataFileDefine.tryGet(fileName, DT_MIN);
	if(fileDefine == DT_MIN)
	{
		LOG_ERROR("error : can not find data file define, file name : %s, filePath : %s", fileName.c_str(), filePath.c_str());
		return;
	}

	// 如果该数据已经存在,并且需要覆盖,则先删除数据
	auto iterDataStruct = mDataStructList.find(fileDefine);
	if (iterDataStruct != mDataStructList.end())
	{
		if (forceCover)
		{
			destroyData(iterDataStruct->first);
		}
		else
		{
			return;
		}
	}

	// 查找工厂
	DataFactoryBase* factory = getDataFactory(fileDefine);
	if(factory == NULL)
	{
		LOG_ERROR("error : can not find factory, type : %d, filename : %s, filePath : %s", (int)fileDefine, fileName.c_str(), filePath.c_str());
		return;
	}

	// 打开文件
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(SystemUtility::getAvailableResourcePath(filePath), &fileSize);

	// 解析文件
	txVector<Data*> dataList;
	const int& dataSize = factory->getDataSize();
	int dataCount = fileSize / dataSize;
	for (int i = 0; i < dataCount; ++i)
	{
		Data* newData = factory->createData();
		if (newData == NULL)
		{
			TRACE_DELETE_ARRAY(fileBuffer);
			LOG_ERROR("error : can not create data ,type : %d", factory->getType());
			return;
		}
		if(newData->read(fileBuffer + i * dataSize, dataSize))
		{
			dataList.push_back(newData);
		}
	}
	mDataStructList.insert(factory->getType(), dataList);
	TRACE_DELETE_ARRAY(fileBuffer);
}

bool DataBase::writeBinaryFile(DATA_TYPE type)
{
	auto iterData = mDataStructList.find(type);
	if (iterData == mDataStructList.end())
	{
		return false;
	}

	auto factory = getDataFactory(type);
	if (factory == NULL)
	{
		LOG_ERROR("error : can not find data factory! type : %d", type);
		return false;
	}

	int dataCount = iterData->second.size();
	int dataSize = factory->getDataSize();
	int writeBufferSize = dataSize * dataCount;
	char* writeFileBuffer = TRACE_NEW_ARRAY(char, writeBufferSize, writeFileBuffer);
	FOR(iterData->second, int i = 0; i < dataCount; ++i)
	{
		iterData->second[i]->write(writeFileBuffer + i * dataSize, dataSize);
	}
	END(iterData->second);

	// 将缓冲写入文件
	auto iterDataDefine = mDataDefineFile.find(type);
	if (iterDataDefine == mDataDefineFile.end())
	{
		LOG_ERROR("error : can not find data type in data define file list! type : %d", type);
		return false;
	}
	FileUtility::writeFile(SystemUtility::getAvailableResourcePath(GAME_DATA_PATH + iterDataDefine->second + ".table"), writeFileBuffer, writeBufferSize);
	TRACE_DELETE_ARRAY(writeFileBuffer);
	return true;
}
