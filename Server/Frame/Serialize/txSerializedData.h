#ifndef _TX_SERIALIZED_DATA_H_
#define _TX_SERIALIZED_DATA_H_

#include "ServerDefine.h"
#include "GameBase.h"

struct DataParameter
{
	char* mDataPtr;
	int mDataSize;
	string mDataType;
	string mDescribe;
	DataParameter()
	{
		mDataPtr = NULL;
		mDataSize = 0;
	}
	DataParameter(char* ptr, int dataSize, const string& dataType, const string& describe)
	{
		mDataPtr = ptr;
		mDataSize = dataSize;
		mDataType = dataType;
		mDescribe = describe;
	}
};

class txSerializedData : public GameBase
{
public:
	txSerializedData()
	{
		mDataSize = 0;
	}
	virtual ~txSerializedData() { mDataParameterList.clear(); }
	virtual bool readFromBuffer(char* pBuffer, int bufferSize);
	virtual bool writeToBuffer(char* pBuffer, int bufferSize);
	virtual bool writeData(const string& dataString, int paramIndex);
	virtual bool writeData(char* buffer, int bufferSize, int paramIndex);
	string getValueString(int paramIndex);
	bool readStringList(const vector<string>& dataList);
	int getSize() { return mDataSize; }
	// 在子类构造中调用
	virtual void fillParams() = 0;
	void zeroParams();
	template<typename T>
	void pushParam(T& param, const string& describe = EMPTY_STRING)
	{
		mDataParameterList.push_back(DataParameter((char*)&param, sizeof(param), typeid(T).name(), describe));
	}
	template<typename T>
	void pushArrayParam(const T* param, int count, const string& describe = EMPTY_STRING)
	{
		if (count > 0)
		{
			mDataParameterList.push_back(DataParameter((char*)param, sizeof(param[0]) * count, typeid(T*).name(), describe));
		}
	}
public:
	vector<DataParameter> mDataParameterList;
	int mDataSize;
	static string mCharType;
	static string mByteType;
	static string mIntType;
	static string mShortType;
	static string mFloatType;
	static string mCharArrayType;
	static string mIntArrayType;
};

#endif