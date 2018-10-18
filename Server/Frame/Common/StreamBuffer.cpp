#include "ServerDefine.h"
#include "txMemoryTrace.h"
#include "SystemUtility.h"
#include "BinaryUtility.h"
#include "StreamBuffer.h"

StreamBuffer::StreamBuffer(int bufferSize)
{
	mBufferSize = 0;
	mBuffer = NULL;
	mDataLength = 0;
	mDataStart = 0;
	resizeBuffer(bufferSize);
	clearInputBuffer();
}
StreamBuffer::~StreamBuffer()
{
	TRACE_DELETE_ARRAY(mBuffer);
}
bool StreamBuffer::isAvailable(int count)
{
	return count <= mBufferSize - mDataLength;
}
void StreamBuffer::addDataToInputBuffer(const char* data, int count, bool clearIfFull)
{
	// 缓冲区空闲部分足够放下数据时才处理
	if (count <= mBufferSize - (mDataStart + mDataLength))
	{
		memcpy(mBuffer + mDataStart + mDataLength, data, count);
		mDataLength += count;
	}
	// 空闲部分不够,查看所有空闲部分是否足够,如果足够,则调整缓冲区
	else if (count <= mBufferSize - mDataLength)
	{
		memmove(mBuffer, mBuffer + mDataStart, mDataLength);
		mDataStart = 0;
		addDataToInputBuffer(data, count);
	}
	// 整个缓冲区不足,根据参数清空缓冲区或者提示缓冲区太小
	else
	{
		if (clearIfFull && mBufferSize >= count)
		{
			clearInputBuffer();
			addDataToInputBuffer(data, count);
		}
		else
		{
			LOG_INFO("缓冲区太小");
		}
	}
}
void StreamBuffer::removeDataFromInputBuffer(int count)
{
	if (mDataLength >= count)
	{
		mDataStart += count;
		mDataLength -= count;
	}
	else
	{
		LOG_INFO("删除数据失败!数据量不足");
	}
}
void StreamBuffer::clearInputBuffer()
{
	mDataLength = 0;
	mDataStart = 0;
}

void StreamBuffer::resizeBuffer(int size)
{
	if (mBufferSize >= size)
	{
		return;
	}
	mBufferSize = size;
	if (mBuffer != NULL)
	{
		// 创建新的缓冲区,将原来的数据拷贝到新缓冲区中,销毁原缓冲区,指向新缓冲区
		char* newBuffer = TRACE_NEW_ARRAY(char, mBufferSize, newBuffer);
		if (mDataLength > 0)
		{
			memcpy(newBuffer, mBuffer + mDataStart, mDataLength);
		}
		TRACE_DELETE_ARRAY(mBuffer);
		mBuffer = newBuffer;
		mDataStart = 0;
	}
	else
	{
		TRACE_NEW_ARRAY(char, mBufferSize, mBuffer);
	}
}