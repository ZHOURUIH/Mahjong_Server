#ifndef _TX_SERIALIZER_H_
#define _TX_SERIALIZER_H_

#include "GameLog.h"

// 用于生成二进制文件的
class txSerializer
{
public:
	txSerializer(const bool& traceMemery = true);
	txSerializer(char* buffer, const int& bufferSize);
	virtual ~txSerializer();
	template<typename T>
	void write(const T& value)
	{
		// 如果是只读的,则不能写入
		if (!mWriteFlag)
		{
			GAME_ERROR("error : the buffer is read only, can not write!");
			return;
		}
		int writeLen = sizeof(T);
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
		memcpy(mBuffer + mIndex, &value, writeLen);
		mIndex += writeLen;
	}
	template<typename T>
	void read(T& value)
	{
		// 如果是只写的,则不能读取
		if (mWriteFlag)
		{
			GAME_ERROR("error : the buffer is write only, can not read!");
			return;
		}
		if (mBuffer == NULL)
		{
			GAME_ERROR("error : buffer is NULL! can not read");
			return;
		}
		int readLen = sizeof(T);
		if (mIndex + readLen > mBufferSize)
		{
			GAME_ERROR("error : read buffer out of range! cur index : %d, buffer size : %d, read length : %d", mIndex, mBufferSize, readLen);
			return;
		}
		value = *((T*)(mBuffer + mIndex));
		mIndex += readLen;
	}
	void writeBuffer(char* buffer, const int& bufferSize);
	void readBuffer(char* buffer, const int& bufferSize, const int& readLen);
	void writeString(const char* str);
	void readString(char* str, const int& strBufferSize);
	void writeToFile(const std::string& fullName);
	const char* getBuffer() const { return mBuffer; }
	const int& getBufferSize() { return mBufferSize; }
	const int& getDataSize() { return mIndex; }
protected:
	void resizeBuffer(const int& maxSize);
	void createBuffer(const int& bufferSize);
protected:
	char* mBuffer;
	int mIndex;
	int mBufferSize;
	bool mWriteFlag;	// 如果为真,则表示是只写的,为假则表示是只读的
	bool mTraceMemery;	// 为真则表示所有的内存申请释放操作都会被记录下来,为假则不会被记录,在记录内存跟踪信息时会用到
};

#endif
