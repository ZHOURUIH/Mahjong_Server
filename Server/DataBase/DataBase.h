#ifndef _DATA_BASE_H_
#define _DATA_BASE_H_

#include "txCommandReceiver.h"

#include "DataDefine.h"
#include "DataFactory.h"
#include "ServerBase.h"

class Data;
class DataFactoryBase;

class DataBase : public txCommandReceiver, public ServerBase
{
public:
	DataBase()
		:
		txCommandReceiver("DataBase")
	{}
	virtual ~DataBase() { destroy(); }
	// 鍒濆鍖栨墍鏈夋暟鎹?
	void init(const bool& loadAllData = true);
	virtual void initDataFactory() = 0;
	void destroyDataFactory();
	Data* createData(const DATA_TYPE& type);
	void loadAllDataFromFile();
	void destroyAllData();
	void destroyData(const DATA_TYPE& type);
	void loadData(const std::string& filePath, const bool& forceCover);
	bool writeBinaryFile(const DATA_TYPE& type);
	virtual void update(const float& elapsedTime){}
	void destroy()
	{
		// 鍏堥攢姣佹暟鎹?
		destroyAllData();
		// 鍐嶉攢姣佸伐鍘?
		destroyDataFactory();
		mDataFileDefine.clear();
		mDataDefineFile.clear();
	}
	// 寰楀埌鏁版嵁鍒楄〃
	bool getDataList(const DATA_TYPE& type, txVector<Data*>& dataList)
	{
		txMap<DATA_TYPE, txVector<Data*> >::iterator iter = mDataStructList.find(type);
		if (iter == mDataStructList.end())
		{
			return false;
		}
		dataList = iter->second;
		return true;
	}
	// 寰楀埌鏁版嵁鏁伴噺
	int getDataCount(const DATA_TYPE& type)
	{
		txMap<DATA_TYPE, txVector<Data*> >::iterator iter = mDataStructList.find(type);
		if (iter != mDataStructList.end())
		{
			return (int)iter->second.size();
		}
		return 0;
	}
	// 鏌ヨ鏁版嵁
	Data* queryData(const DATA_TYPE& type, const int& index)
	{
		txMap<DATA_TYPE, txVector<Data*> >::iterator iter = mDataStructList.find(type);
		if (iter != mDataStructList.end())
		{
			if (index < (int)iter->second.size())
			{
				return (iter->second)[index];
			}
		}
		return NULL;
	}
	bool addData(const DATA_TYPE& type, Data* data, const int& pos = -1);
	bool deleteData(const DATA_TYPE& type, const int& index);
	// 鏍规嵁鏁版嵁鍚嶅緱鍒版暟鎹畾涔?
	const std::string& getDataNameByDataType(const DATA_TYPE& type)
	{
		txMap<DATA_TYPE, std::string>::iterator iter = mDataDefineFile.find(type);
		if (iter != mDataDefineFile.end())
		{
			return iter->second;
		}
		return EMPTY_STRING;
	}
	// 鏍规嵁鏁版嵁瀹氫箟寰楀埌鏁版嵁鍚?
	DATA_TYPE getDataTypeByDataName(const std::string& name)
	{
		txMap<std::string, DATA_TYPE>::iterator iter = mDataFileDefine.find(name);
		if (iter != mDataFileDefine.end())
		{
			return iter->second;
		}
		return DT_MIN;
	}
protected:
	template<typename T>
	void addDataFactory(const DATA_TYPE& type, const std::string& dataName)
	{
		T data(type);
		mDataFileDefine.insert(dataName, type);
		mDataDefineFile.insert(type, dataName);
		DataFactoryBase* factory = DataFactoryBase::createDataFactory<DataFactory<T> >(type, data.getSize());
		addDataFactoryToList(factory);
	}
	void addDataFactoryToList(DataFactoryBase* factory);
	DataFactoryBase* getDataFactory(const DATA_TYPE& type)
	{
		txMap<DATA_TYPE, DataFactoryBase*>::iterator itrFind = mDataFactoryList.find(type);
		if (itrFind != mDataFactoryList.end())
		{
			return itrFind->second;
		}
		return NULL;
	}
protected:
	txMap<DATA_TYPE, txVector<Data*> > mDataStructList;
	static txMap<DATA_TYPE, DataFactoryBase*> mDataFactoryList;
	static txMap<std::string, DATA_TYPE> mDataFileDefine;
	static txMap<DATA_TYPE, std::string> mDataDefineFile;
};

#endif
