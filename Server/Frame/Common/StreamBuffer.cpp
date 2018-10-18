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
	// ���������в����㹻��������ʱ�Ŵ���
	if (count <= mBufferSize - (mDataStart + mDataLength))
	{
		memcpy(mBuffer + mDataStart + mDataLength, data, count);
		mDataLength += count;
	}
	// ���в��ֲ���,�鿴���п��в����Ƿ��㹻,����㹻,�����������
	else if (count <= mBufferSize - mDataLength)
	{
		memmove(mBuffer, mBuffer + mDataStart, mDataLength);
		mDataStart = 0;
		addDataToInputBuffer(data, count);
	}
	// ��������������,���ݲ�����ջ�����������ʾ������̫С
	else
	{
		if (clearIfFull && mBufferSize >= count)
		{
			clearInputBuffer();
			addDataToInputBuffer(data, count);
		}
		else
		{
			LOG_INFO("������̫С");
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
		LOG_INFO("ɾ������ʧ��!����������");
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
		// �����µĻ�����,��ԭ�������ݿ������»�������,����ԭ������,ָ���»�����
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