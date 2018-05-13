#include "DataBase.h"
#include "DataFactory.h"
#include "Utility.h"

txMap<DATA_TYPE, DataFactoryBase*> DataBase::mDataFactoryList;
txMap<std::string, DATA_TYPE> DataBase::mDataFileDefine;
txMap<DATA_TYPE, std::string> DataBase::mDataDefineFile;

void DataBase::init(const bool& loadAllData)
{
	initDataFactory();
	if (loadAllData)
	{
		loadAllDataFromFile();
	}
}

void DataBase::addDataFactoryToList(DataFactoryBase* factory)
{
	mDataFactoryList.insert(factory->getType(), factory);
}

void DataBase::destroyDataFactory()
{
	txMap<DATA_TYPE, DataFactoryBase*>::iterator iterDataFactoryList = mDataFactoryList.begin();
	txMap<DATA_TYPE, DataFactoryBase*>::iterator iterDataFactoryListEnd = mDataFactoryList.end();
	FOR_STL(mDataFactoryList, ; iterDataFactoryList != iterDataFactoryListEnd; ++iterDataFactoryList)
	{
		TRACE_DELETE(iterDataFactoryList->second);
	}
	END_FOR_STL(mDataFactoryList);
	mDataFactoryList.clear();
}

bool DataBase::addData(const DATA_TYPE& type, Data* data, const int& pos)
{
	if (data == NULL)
	{
		return false;
	}
	if (data->getType() != type)
	{
		return false;
	}
	txMap<DATA_TYPE, txVector<Data*> >::iterator iter = mDataStructList.find(type);
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

bool DataBase::deleteData(const DATA_TYPE& type, const int& index)
{
	txMap<DATA_TYPE, txVector<Data*> >::iterator iter = mDataStructList.find(type);
	if (iter != mDataStructList.end())
	{
		if (index >= (int)iter->second.size())
		{
			return false;
		}
		DataFactoryBase* dataFactory = getDataFactory(type);
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

Data* DataBase::createData(const DATA_TYPE& type)
{
	DataFactoryBase* factory = getDataFactory(type);
	if (factory != NULL)
	{
		return factory->createData();
	}
	return NULL;
}

void DataBase::loadAllDataFromFile()
{
	// 读取配置文件，获得需要加载的所有数据列表
	// 遍历每一个文件名，加载相应的文件
	txVector<std::string> fileList;
	txFileUtility::findFiles(txUtility::getAvailableResourcePath(GAME_DATA_PATH).c_str(), fileList, ".table");
	int listSize = fileList.size();
	FOR_STL(fileList, int i = 0; i < listSize; ++i)
	{
		loadData(fileList[i], true);
	}
	END_FOR_STL(fileList);
}

void  DataBase::destroyAllData()
{
	txMap<DATA_TYPE, txVector<Data*> >::iterator iterStructList = mDataStructList.begin();
	txMap<DATA_TYPE, txVector<Data*> >::iterator iterStructListEnd = mDataStructList.end();
	FOR_STL(mDataStructList, ; iterStructList != iterStructListEnd; ++iterStructList)
	{
		DataFactoryBase* factory = getDataFactory(iterStructList->first);
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
	END_FOR_STL(mDataStructList);
	mDataStructList.clear();
}

void DataBase::destroyData(const DATA_TYPE& type)
{
	txMap<DATA_TYPE, txVector<Data*> >::iterator iterStructList = mDataStructList.find(type);
	if (iterStructList != mDataStructList.end())
	{
		DataFactoryBase* factory = getDataFactory(iterStructList->first);
		if (factory != NULL)
		{
			int dataCount = iterStructList->second.size();
			FOR_STL(iterStructList->second, int i = 0; i < dataCount; ++i)
			{
				factory->destroyData((iterStructList->second)[i]);
			}
			END_FOR_STL(iterStructList->second);
			iterStructList->second.clear();
		}
		mDataStructList.erase(iterStructList);
	}
}

void DataBase::loadData(const std::string& filePath, const bool& forceCover)
{
	// 根据文件名查找工厂类型
	std::string fileName = txStringUtility::getFileNameNoSuffix(filePath);
	txMap<std::string, DATA_TYPE>::iterator iterFileDefine = mDataFileDefine.find(fileName);
	if(iterFileDefine == mDataFileDefine.end())
	{
		LOG_ERROR("error : can not find data file define, file name : %s, filePath : %s", fileName.c_str(), filePath.c_str());
		return;
	}

	// 如果该数据已经存在,并且需要覆盖,则先删除数据
	txMap<DATA_TYPE, txVector<Data*> >::iterator iterDataStruct = mDataStructList.find(iterFileDefine->second);
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
	DataFactoryBase* factory = getDataFactory(iterFileDefine->second);
	if(factory == NULL)
	{
		LOG_ERROR("error : can not find factory, type : %d, filename : %s, filePath : %s", (int)iterFileDefine->second, fileName.c_str(), filePath.c_str());
		return;
	}

	// 打开文件
	int fileSize = 0;
	char* fileBuffer = txFileUtility::openBinaryFile(txUtility::getAvailableResourcePath(filePath), &fileSize);

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

bool DataBase::writeBinaryFile(const DATA_TYPE& type)
{
	txMap<DATA_TYPE, txVector<Data*> >::iterator iterData = mDataStructList.find(type);
	if (iterData == mDataStructList.end())
	{
		return false;
	}

	DataFactoryBase* factory = getDataFactory(type);
	if (factory == NULL)
	{
		LOG_ERROR("error : can not find data factory! type : %d", type);
		return false;
	}

	int dataCount = iterData->second.size();
	const int& dataSize = factory->getDataSize();
	int writeBufferSize = dataSize * dataCount;
	char* writeFileBuffer = TRACE_NEW_ARRAY(char, writeBufferSize, writeFileBuffer);
	FOR_STL(iterData->second, int i = 0; i < dataCount; ++i)
	{
		Data* pData = iterData->second[i];
		pData->write(writeFileBuffer + i * dataSize, dataSize);
	}
	END_FOR_STL(iterData->second);

	// 将缓冲写入文件
	txMap<DATA_TYPE, std::string>::iterator iterDataDefine = mDataDefineFile.find(type);
	if (iterDataDefine == mDataDefineFile.end())
	{
		LOG_ERROR("error : can not find data type in data define file list! type : %d", type);
		return false;
	}
	txFileUtility::writeFile(txUtility::getAvailableResourcePath(GAME_DATA_PATH + iterDataDefine->second + ".table"), writeBufferSize, writeFileBuffer);
	TRACE_DELETE_ARRAY(writeFileBuffer);
	return true;
}
