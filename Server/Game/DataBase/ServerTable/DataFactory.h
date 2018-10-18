#ifndef _DATA_FACTORY_H_
#define _DATA_FACTORY_H_

#include "DataDefine.h"
#include "txMemoryTrace.h"

#include "Data.h"
#include "GameLog.h"
#include "Utility.h"

class DataFactoryBase
{
public:
	DataFactoryBase(DATA_TYPE type, int dataSize)
		:
		mType(type),
		mCount(0),
		mDataSize(dataSize)
	{}
	virtual ~DataFactoryBase()
	{
		if (mCount != 0)
		{
			LOG_INFO("there is data left! count : " + StringUtility::intToString(mCount) + ", type : " + StringUtility::intToString(mType));
		}
	}
	virtual Data* createData() = 0;
	template<class T>
	static DataFactoryBase* createDataFactory(DATA_TYPE type, int dataSize)
	{
		T* newDataFactory = TRACE_NEW(T, newDataFactory, type, dataSize);
		return newDataFactory;
	}
	void destroyData(Data*& data)
	{
		if (data == NULL)
		{
			LOG_ERROR("can not destroy NULL data!");
			return;
		}
		if (data->getType() != mType)
		{
			LOG_ERROR("destroy wrong type data! factory type : " + StringUtility::intToString(mType) + ", data type : " + StringUtility::intToString(data->getType()));
			return;
		}
		TRACE_DELETE(data);
		--mCount;
	}
	DATA_TYPE getType() { return mType; }
	int getDataSize() { return mDataSize; }
protected:
	DATA_TYPE mType;
	int mCount;
	int mDataSize;
};

template<typename T>
class DataFactory : public DataFactoryBase
{
public:
	DataFactory(DATA_TYPE type, int dataSize)
		:
		DataFactoryBase(type, dataSize)
	{}
	virtual Data* createData()
	{
		T* newData = TRACE_NEW(T, newData, mType);
		++mCount;
		return newData;
	}
};

#endif
