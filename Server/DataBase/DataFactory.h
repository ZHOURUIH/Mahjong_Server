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
	DataFactoryBase(const DATA_TYPE& type, const int& dataSize)
		:
		mType(type),
		mCount(0),
		mDataSize(dataSize)
	{}
	virtual ~DataFactoryBase()
	{
		if (mCount != 0)
		{
			LOG_INFO("error : there is data left! count : %d, type : %d", mCount, (int)mType);
		}
	}
	virtual Data* createData() = 0;
	template<class T>
	static DataFactoryBase* createDataFactory(const DATA_TYPE& type, const int& dataSize)
	{
		T* newDataFactory = TRACE_NEW(T, newDataFactory, type, dataSize);
		return newDataFactory;
	}
	void destroyData(Data*& data)
	{
		if (data == NULL)
		{
			GAME_ERROR("error : can not destroy NULL data!");
			return;
		}
		if (data->getType() != mType)
		{
			GAME_ERROR("error : destroy wrong type data! factory type : %d, data type : %d", mType, data->getType());
			return;
		}
		TRACE_DELETE(data);
		--mCount;
	}
	const DATA_TYPE& getType() { return mType; }
	const int& getDataSize() { return mDataSize; }
protected:
	DATA_TYPE mType;
	int mCount;
	int mDataSize;
};

template<typename T>
class DataFactory : public DataFactoryBase
{
public:
	DataFactory(const DATA_TYPE& type, const int& dataSize)
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
