#include "txSerializedData.h"
#include "txUtility.h"

std::string txSerializedData::mIntType = typeid(int).name();
std::string txSerializedData::mShortType = typeid(short).name();
std::string txSerializedData::mFloatType = typeid(float).name();
std::string txSerializedData::mCharArrayType = typeid(char*).name();
std::string txSerializedData::mIntArrayType = typeid(int*).name();

bool txSerializedData::read(char* pBuffer, const int& bufferSize)
{
	int bufferOffset = 0;
	bool ret = true;
	int parameterCount = mDataParameterList.size();
	FOR_STL(mDataParameterList, int i = 0; i < parameterCount; ++i)
	{
		ret = ret && readByte(mDataParameterList[i].mDataPtr, pBuffer, bufferOffset, bufferSize, mDataParameterList[i].mDataSize);
	}
	END_FOR_STL(mDataParameterList);
	return ret;
}

bool txSerializedData::write(char* pBuffer, const int& bufferSize)
{
	int curWriteSize = 0;
	bool ret = true;
	int parameterCount = mDataParameterList.size();
	FOR_STL(mDataParameterList, int i = 0; i < parameterCount; ++i)
	{
		ret = ret && writeByte(pBuffer, mDataParameterList[i].mDataPtr, curWriteSize, bufferSize, mDataParameterList[i].mDataSize);
	}
	END_FOR_STL(mDataParameterList);
	return ret;
}

bool txSerializedData::writeData(const std::string& dataString, const int& paramIndex)
{
	if (paramIndex < 0 || paramIndex >= (int)mDataParameterList.size())
	{
		return false;
	}
	const DataParameter& dataParam = mDataParameterList[paramIndex];
	const std::string& paramType = dataParam.mDataType;
	if (paramType == mIntType)
	{
		*(int*)(dataParam.mDataPtr) = txUtility::stringToInt(dataString);
	}
	else if (paramType == mShortType)
	{
		*(short*)(dataParam.mDataPtr) = txUtility::stringToInt(dataString);
	}
	else if (paramType == mFloatType)
	{
		*(float*)(dataParam.mDataPtr) = txUtility::stringToFloat(dataString);
	}
	else if (paramType == mCharArrayType)
	{
		memset(dataParam.mDataPtr, 0, dataParam.mDataSize);
		int copySize = TX_MIN((int)dataString.length(), dataParam.mDataSize - 1);
		memcpy(dataParam.mDataPtr, dataString.c_str(), copySize);
	}
	else if (paramType == mIntArrayType)
	{
		txVector<std::string> valueList;
		txUtility::split(dataString, ";", &valueList);
		int valueCount = valueList.size();
		FOR_STL(valueList, int i = 0; i < valueCount; ++i)
		{
			((int*)(dataParam.mDataPtr))[i] = txUtility::stringToInt(valueList[i]);
		}
		END_FOR_STL(valueList);
	}
	return true;
}

bool txSerializedData::writeData(char* buffer, const int& bufferSize, const int& paramIndex)
{
	if (buffer == NULL || paramIndex < 0 || paramIndex >= (int)mDataParameterList.size())
	{
		return false;
	}
	if (bufferSize < mDataParameterList[paramIndex].mDataSize)
	{
		return false;
	}
	memcpy(mDataParameterList[paramIndex].mDataPtr, buffer, mDataParameterList[paramIndex].mDataSize);
	return true;
}

std::string txSerializedData::getValueString(const int& paramIndex)
{
	const DataParameter& dataParam = mDataParameterList[paramIndex];
	std::string dataString;
	if (dataParam.mDataType == mIntType)
	{
		dataString = txUtility::intToString(*((int*)dataParam.mDataPtr));
	}
	else if (dataParam.mDataType == mShortType)
	{
		dataString = txUtility::intToString((int)*((short*)dataParam.mDataPtr));
	}
	else if (dataParam.mDataType == mFloatType)
	{
		dataString = txUtility::floatToString(*((float*)dataParam.mDataPtr), 2);
	}
	else if (dataParam.mDataType == mCharArrayType)
	{
		dataString = dataParam.mDataPtr;
	}
	else if (dataParam.mDataType == mIntArrayType)
	{
		int intCount = dataParam.mDataSize / sizeof(int);
		for (int i = 0; i < intCount; ++i)
		{
			dataString += txUtility::intToString(*(int*)(dataParam.mDataPtr + i * sizeof(int)));
			if (i + 1 < intCount)
			{
				dataString += ";";
			}
		}
	}
	return dataString;
}

void txSerializedData::zeroParams()
{
	// 数据内容全部清空时,也一起计算数据大小
	mDataSize = 0;
	int parameterCount = mDataParameterList.size();
	FOR_STL(mDataParameterList, int i = 0; i < parameterCount; ++i)
	{
		memset(mDataParameterList[i].mDataPtr, 0, mDataParameterList[i].mDataSize);
		mDataSize += mDataParameterList[i].mDataSize;
	}
	END_FOR_STL(mDataParameterList);
}

void txSerializedData::readStringList(txVector<std::string>& dataList)
{
	int curIndex = 0;
	int parameterCount = mDataParameterList.size();
	FOR_STL(mDataParameterList, int i = 0; i < parameterCount; ++i)
	{
		if (curIndex >= (int)dataList.size())
		{
			break;
		}
		const DataParameter& paramter = mDataParameterList[i];
		if (paramter.mDataType == mIntType)
		{
			*(int*)(paramter.mDataPtr) = txUtility::stringToInt(dataList[curIndex]);
		}
		else if (paramter.mDataType == mShortType)
		{
			*(short*)(paramter.mDataPtr) = (short)txUtility::stringToInt(dataList[curIndex]);
		}
		else if (paramter.mDataType == mFloatType)
		{
			*(float*)(paramter.mDataPtr) = txUtility::stringToFloat(dataList[curIndex]);
		}
		else if (paramter.mDataType == mCharArrayType)
		{
			memset(mDataParameterList[i].mDataPtr, 0, mDataParameterList[i].mDataSize);
			int copySize = TX_MIN((int)dataList[curIndex].length(), mDataParameterList[i].mDataSize - 1);
			memcpy(mDataParameterList[i].mDataPtr, dataList[curIndex].c_str(), copySize);
		}
		else if (paramter.mDataType == mIntArrayType)
		{
			txVector<std::string> breakVec;
			txUtility::split(dataList[curIndex], ";", &breakVec);
			int size = breakVec.size();
			FOR_STL(breakVec, int j = 0; j < size; ++j)
			{
				((int*)(paramter.mDataPtr))[j] = txUtility::stringToInt(breakVec[j]);
			}
			END_FOR_STL(breakVec);
		}
		++curIndex;
	}
	END_FOR_STL(mDataParameterList);
}