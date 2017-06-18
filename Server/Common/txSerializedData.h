#ifndef _TX_SERIALIZED_DATA__H_
#define _TX_SERIALIZED_DATA__H_

#include "ServerDefine.h"

struct DataParameter
{
	char* mDataPtr;
	int mDataSize;
	std::string mDataType;
	std::string mDescribe;
	DataParameter()
	{
		mDataPtr = NULL;
		mDataSize = 0;
	}
	DataParameter(char* ptr, const int& dataSize, const std::string& dataType, const std::string& describe)
	{
		mDataPtr = ptr;
		mDataSize = dataSize;
		mDataType = dataType;
		mDescribe = describe;
	}
};

class txSerializedData
{
public:
	txSerializedData()
	{
		mDataSize = 0;
	}
	virtual ~txSerializedData() { mDataParameterList.clear(); }
	virtual bool read(char* pBuffer, const int& bufferSize);
	virtual bool write(char* pBuffer, const int& bufferSize);
	virtual bool writeData(const std::string& dataString, const int& paramIndex);
	virtual bool writeData(char* buffer, const int& bufferSize, const int& paramIndex);
	std::string getValueString(const int& paramIndex);
	bool readStringList(const std::vector<std::string>& dataList);
	const int& getSize() { return mDataSize; }
	static bool readByte(char* dest, char* source, int& curSourceOffset, const int& sourceSize, const int& readSize)
	{
		if (curSourceOffset + readSize > sourceSize)
		{
			return false;
		}
		memcpy(dest, source + curSourceOffset, readSize);
		curSourceOffset += readSize;
		return true;
	}
	static bool writeByte(char* dest, char* source, int& destOffset, const int& destSize, const int& writeSize)
	{
		if (destOffset + writeSize > destSize)
		{
			return false;
		}
		memcpy(dest + destOffset, source, writeSize);
		destOffset += writeSize;
		return true;
	}
	// 在子类构造中调用
	virtual void fillParams() = 0;
	void zeroParams();
	template<typename T>
	void pushParam(const T& param, const std::string& describe = EMPTY_STRING)
	{
		mDataParameterList.push_back(DataParameter((char*)&param, sizeof(param), typeid(T).name(), describe));
	}
	template<typename T>
	void pushArrayParam(const T* param, const int& count, const std::string& describe = EMPTY_STRING)
	{
		if (count > 0)
		{
			mDataParameterList.push_back(DataParameter((char*)param, sizeof(param[0]) * count, typeid(T*).name(), describe));
		}
	}
public:
	std::vector<DataParameter> mDataParameterList;
	int mDataSize;
	static std::string mIntType;
	static std::string mShortType;
	static std::string mFloatType;
	static std::string mCharArrayType;
	static std::string mIntArrayType;
};

#endif