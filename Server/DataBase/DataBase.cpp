#include "txDataManager.h"
#include "txDataElem.h"

#include "DataBase.h"
#include "DataFactory.h"

std::map<DATA_TYPE, DataFactoryBase*> DataBase::mDataFactoryList;
std::map<std::string, DATA_TYPE> DataBase::mDataFileDefine;
std::map<DATA_TYPE, std::string> DataBase::mDataDefineFile;

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
	mDataFactoryList.insert(std::make_pair(factory->getType(), factory));
}

void DataBase::destroyDataFactory()
{
	std::map<DATA_TYPE, DataFactoryBase*>::iterator iterDataFactoryList = mDataFactoryList.begin();
	std::map<DATA_TYPE, DataFactoryBase*>::iterator iterDataFactoryListEnd = mDataFactoryList.end();
	for (; iterDataFactoryList != iterDataFactoryListEnd; ++iterDataFactoryList)
	{
		TRACE_DELETE(iterDataFactoryList->second);
	}
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
	std::map<DATA_TYPE, std::vector<Data*> >::iterator iter = mDataStructList.find(type);
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
		std::vector<Data*> dataList;
		dataList.push_back(data);
		mDataStructList.insert(std::make_pair(type, dataList));
	}
	return true;
}

bool DataBase::deleteData(const DATA_TYPE& type, const int& index)
{
	std::map<DATA_TYPE, std::vector<Data*> >::iterator iter = mDataStructList.find(type);
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
	std::vector<std::string> fileList;
	std::vector<std::string> patterns;
	patterns.push_back(".table");
	txUtility::findFiles(txUtility::getAvailableResourcePath(GAME_DATA_PATH).c_str(), fileList, patterns);
	int listSize = fileList.size();
	for(int i = 0; i < listSize; ++i)
	{
		loadData(fileList[i], true);
	}
}

void  DataBase::destroyAllData()
{
	std::map<DATA_TYPE, std::vector<Data*> >::iterator iterStructList = mDataStructList.begin();
	std::map<DATA_TYPE, std::vector<Data*> >::iterator iterStructListEnd = mDataStructList.end();
	for(; iterStructList != iterStructListEnd; ++iterStructList)
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
			GAME_ERROR("error : can not find data factory : %d, DataBase::destroyAllData", (int)iterStructList->first);
		}
	}
	mDataStructList.clear();
}

void DataBase::destroyData(const DATA_TYPE& type)
{
	std::map<DATA_TYPE, std::vector<Data*> >::iterator iterStructList = mDataStructList.find(type);
	if (iterStructList != mDataStructList.end())
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
		mDataStructList.erase(iterStructList);
	}
}

void DataBase::loadData(const std::string& filePath, const bool& forceCover)
{
	// 根据文件名查找工厂类型
	std::string fileName = txUtility::getFileNameNoSuffix(filePath);
	std::map<std::string, DATA_TYPE>::iterator iterFileDefine = mDataFileDefine.find(fileName);
	if(iterFileDefine == mDataFileDefine.end())
	{
		GAME_ERROR("error : can not find data file define, file name : %s, filePath : %s", fileName.c_str(), filePath.c_str());
		return;
	}

	// 如果该数据已经存在,并且需要覆盖,则先删除数据
	std::map<DATA_TYPE, std::vector<Data*> >::iterator iterDataStruct = mDataStructList.find(iterFileDefine->second);
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
		GAME_ERROR("error : can not find factory, type : %d, filename : %s, filePath : %s", (int)iterFileDefine->second, fileName.c_str(), filePath.c_str());
		return;
	}

	// 打开文件
	int dataIndex = mDataManager->LoadData(filePath.c_str());
	if(dataIndex < 0)
	{
		return;
	}
	char* fileBuffer = mDataManager->GetData(dataIndex)->getValuePtr();
	int fileSize = mDataManager->GetFileSize(dataIndex);

	// 解析文件
	std::vector<Data*> dataList;
	const int& dataSize = factory->getDataSize();
	int dataCount = fileSize / dataSize;
	for (int i = 0; i < dataCount; ++i)
	{
		Data* newData = factory->createData();
		if(NULL == newData)
		{
			mDataManager->DestroyData(dataIndex);
			GAME_ERROR("error : can not create data ,type : %d", factory->getType());
			return;
		}
		if(newData->read(fileBuffer + i * dataSize, dataSize))
		{
			dataList.push_back(newData);
		}
	}

	mDataStructList.insert(std::pair<DATA_TYPE, std::vector<Data*> >(factory->getType(), dataList));
	mDataManager->DestroyData(dataIndex);
}

bool DataBase::writeBinaryFile(const DATA_TYPE& type)
{
	std::map<DATA_TYPE, std::vector<Data*> >::iterator iterData = mDataStructList.find(type);
	if (iterData == mDataStructList.end())
	{
		return false;
	}

	DataFactoryBase* factory = getDataFactory(type);
	if (factory == NULL)
	{
		GAME_ERROR("error : can not find data factory! type : %d", type);
		return false;
	}

	int dataCount = iterData->second.size();
	const int& dataSize = factory->getDataSize();
	int writeBufferSize = dataSize * dataCount;
	char* writeFileBuffer = TRACE_NEW_ARRAY(char, writeBufferSize, writeFileBuffer);
	for (int i = 0; i < dataCount; ++i)
	{
		Data* pData = iterData->second[i];
		pData->write(writeFileBuffer + i * dataSize, dataSize);
	}

	// 将缓冲写入文件
	std::map<DATA_TYPE, std::string>::iterator iterDataDefine = mDataDefineFile.find(type);
	if (iterDataDefine == mDataDefineFile.end())
	{
		GAME_ERROR("error : can not find data type in data define file list! type : %d", type);
		return false;
	}
	txUtility::writeFile(txUtility::getAvailableResourcePath(GAME_DATA_PATH + iterDataDefine->second + ".table"), writeBufferSize, writeFileBuffer);
	TRACE_DELETE_ARRAY(writeFileBuffer);
	return true;
}
