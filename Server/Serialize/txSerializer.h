﻿#ifndef _TX_SERIALIZER_H_
#define _TX_SERIALIZER_H_

#include "ServerDefine.h"
#include "GameLog.h"
#include "Utility.h"

// 用于生成二进制文件的
class txSerializer
{
public:
	txSerializer(bool traceMemery = true);
	txSerializer(char* buffer, int bufferSize);
	virtual ~txSerializer();
	template<typename T>
	void write(T value)
	{
		if (!writeCheck(sizeof(T)))
		{
			return;
		}
		BinaryUtility::write<T>(mBuffer, mBufferSize, mIndex, value);
	}
	template<typename T>
	void read(T& value)
	{
		if (!checkRead(sizeof(T)))
		{
			return;
		}
		value = BinaryUtility::read<T>(mBuffer, mBufferSize, mIndex);
	}
	void writeBuffer(char* buffer, int bufferSize);
	void readBuffer(char* buffer, int readLen);
	void writeString(const char* str);
	void readString(char* str);
	void writeToFile(const std::string& fullName);
	const char* getBuffer() const { return mBuffer; }
	int getBufferSize() { return mBufferSize; }
	int getDataSize() { return mIndex; }
protected:
	void resizeBuffer(int maxSize);
	void createBuffer(int bufferSize);
	bool checkRead(int readLen)
	{
		// 如果是只写的,则不能读取
		if (mWriteFlag)
		{
			LOG_ERROR("error : the buffer is write only, can not read!");
			return false;
		}
		if (mBuffer == NULL)
		{
			LOG_ERROR("error : buffer is NULL! can not read");
			return false;
		}
		if (mIndex + readLen > mBufferSize)
		{
			LOG_ERROR("error : read buffer out of range! cur index : %d, buffer size : %d, read length : %d", mIndex, mBufferSize, readLen);
			return false;
		}
		return true;
	}
	bool writeCheck(int writeLen)
	{
		// 如果是只读的,则不能写入
		if (!mWriteFlag)
		{
			LOG_ERROR("error : the buffer is read only, can not write!");
			return false;
		}
		// 如果缓冲区为空,则创建缓冲区
		if (mBuffer == NULL)
		{
			createBuffer(writeLen);
		}
		// 如果缓冲区已经不够用了,则重新扩展缓冲区
		else if (writeLen + mIndex > mBufferSize)
		{
			resizeBuffer(writeLen + mIndex);
		}
		return true;
	}
protected:
	char* mBuffer;
	int mIndex;
	int mBufferSize;
	bool mWriteFlag;	// 如果为真,则表示是只写的,为假则表示是只读的
	bool mTraceMemery;	// 为真则表示所有的内存申请释放操作都会被记录下来,为假则不会被记录,在记录内存跟踪信息时会用到
};

#endif
