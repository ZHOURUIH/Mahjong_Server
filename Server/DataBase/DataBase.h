#ifndef _DATA_BASE_H_
#define _DATA_BASE_H_

#include "txCommandReceiver.h"

#include "DataDefine.h"
#include "DataFactory.h"
#include "FrameComponent.h"

class Data;
class DataFactoryBase;
class DataBase : public FrameComponent
{
public:
	DataBase(const std::string& name)
		:FrameComponent(name){}
	virtual ~DataBase() { destroy(); }
	// 初始化所有数据
	virtual void init();
	virtual void initDataFactory() {}
	void destroyDataFactory();
	Data* createData(DATA_TYPE type);
	void loadAllDataFromFile();
	void destroyAllData();
	void destroyData(DATA_TYPE type);
	void loadData(const std::string& filePath, bool forceCover);
	bool writeBinaryFile(DATA_TYPE type);
	virtual void update(float elapsedTime){}
	void destroy()
	{
		// 先销毁数据
		destroyAllData();
		// 再销毁工厂
		destroyDataFactory();
		mDataFileDefine.clear();
		mDataDefineFile.clear();
	}
	// 得到数据列表
	bool getDataList(DATA_TYPE type, txVector<Data*>& dataList)
	{
		auto iter = mDataStructList.find(type);
		if (iter == mDataStructList.end())
		{
			return false;
		}
		dataList = iter->second;
		return true;
	}
	// 得到数据数量
	int getDataCount(DATA_TYPE type)
	{
		auto iter = mDataStructList.find(type);
		if (iter != mDataStructList.end())
		{
			return (int)iter->second.size();
		}
		return 0;
	}
	// 查询数据
	Data* queryData(DATA_TYPE type, int index)
	{
		auto iter = mDataStructList.find(type);
		if (iter != mDataStructList.end())
		{
			if (index < (int)iter->second.size())
			{
				return (iter->second)[index];
			}
		}
		return NULL;
	}
	bool addData(DATA_TYPE type, Data* data, int pos = -1);
	bool deleteData(DATA_TYPE type, int index);
	// 根据数据名得到数据定义
	const std::string& getDataNameByDataType(DATA_TYPE type)
	{
		auto iter = mDataDefineFile.find(type);
		if (iter != mDataDefineFile.end())
		{
			return iter->second;
		}
		return EMPTY_STRING;
	}
	// 根据数据定义得到数据名
	DATA_TYPE getDataTypeByDataName(const std::string& name)
	{
		auto iter = mDataFileDefine.find(name);
		if (iter != mDataFileDefine.end())
		{
			return iter->second;
		}
		return DT_MIN;
	}
protected:
	template<typename T>
	void addDataFactory(DATA_TYPE type, const std::string& dataName)
	{
		T data(type);
		mDataFileDefine.insert(dataName, type);
		mDataDefineFile.insert(type, dataName);
		auto factory = DataFactoryBase::createDataFactory<DataFactory<T> >(type, data.getSize());
		addDataFactoryToList(factory);
	}
	void addDataFactoryToList(DataFactoryBase* factory);
	DataFactoryBase* getDataFactory(DATA_TYPE type)
	{
		auto itrFind = mDataFactoryList.find(type);
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
