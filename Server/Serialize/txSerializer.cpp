﻿#include "txSerializer.h"
#include "txUtility.h"
#include "txMemoryTrace.h"
#include "txFileUtility.h"

txSerializer::txSerializer(const bool& traceMemery)
:
mBuffer(NULL),
mIndex(0),
mBufferSize(0),
mWriteFlag(true),
mTraceMemery(traceMemery)
{}

txSerializer::txSerializer(char* buffer, const int& bufferSize)
:
mBuffer(buffer),
mIndex(0),
mBufferSize(bufferSize),
mWriteFlag(false),
mTraceMemery(true)
{}

txSerializer::~txSerializer()
{
	if (mWriteFlag)
	{
		if (mTraceMemery)
		{
			TRACE_DELETE_ARRAY(mBuffer);
		}
		else
		{
			NORMAL_DELETE_ARRAY(mBuffer);
		}
	}
}

void txSerializer::writeBuffer(char* buffer, const int& bufferSize)
{
	// 如果是只读的,则不能写入
	if (!mWriteFlag)
	{
		GAME_ERROR("error : the buffer is read only, can not write!");
		return;
	}
	// 如果缓冲区为空,则创建缓冲区
	if (mBuffer == NULL)
	{
		createBuffer(bufferSize);
	}
	// 如果缓冲区已经不够用了,则重新扩展缓冲区
	else if (bufferSize + mIndex > mBufferSize)
	{
		resizeBuffer(bufferSize + mIndex);
	}
	memcpy(mBuffer + mIndex, buffer, bufferSize);
	mIndex += bufferSize;
}

void txSerializer::readBuffer(char* buffer, const int& bufferSize, const int& readLen)
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
	// 先读入字符串长度
	if (mIndex + readLen > mBufferSize)
	{
		GAME_ERROR("error : read buffer out of range! cur index : %d, buffer size : %d, read length : %d", mIndex, mBufferSize, readLen);
		return;
	}
	// 如果存放数据的空间大小不足以放入当前要读取的数据,则只拷贝能容纳的长度,但是下标应该正常跳转
	if (bufferSize <= readLen)
	{
		memcpy(buffer, mBuffer + mIndex, bufferSize);
		mIndex += readLen;
	}
	else
	{
		memcpy(buffer, mBuffer + mIndex, readLen);
		mIndex += readLen;
	}
}

void txSerializer::writeString(const char* str)
{
	// 如果是只读的,则不能写入
	if (!mWriteFlag)
	{
		GAME_ERROR("error : the buffer is read only, can not write!");
		return;
	}
	// 先写入字符串长度
	int writeLen = strlen(str);
	write(writeLen);
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
	memcpy(mBuffer + mIndex, str, writeLen);
	mIndex += writeLen;
}

void txSerializer::readString(char* str, const int& strBufferSize)
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
	// 先读入字符串长度
	int readLen = 0;
	read(readLen);
	if (mIndex + readLen > mBufferSize)
	{
		GAME_ERROR("error : read buffer out of range! cur index : %d, buffer size : %d, read length : %d", mIndex, mBufferSize, readLen);
		return;
	}
	// 如果存放字符串的空间大小不足以放入当前要读取的字符串,则只拷贝能容纳的长度,但是下标应该正常跳转
	if (strBufferSize <= readLen)
	{
		memcpy(str, mBuffer + mIndex, strBufferSize - 1);
		mIndex += readLen;
		// 加上结束符
		str[strBufferSize - 1] = 0;
	}
	else
	{
		memcpy(str, mBuffer + mIndex, readLen);
		mIndex += readLen;
		// 加上结束符
		str[readLen] = 0;
	}
}

void txSerializer::writeToFile(const std::string& fullName)
{
	// 确保是只写的,并且数据不为空
	if (!mWriteFlag || mBuffer == NULL || mIndex <= 0)
	{
		return;
	}
	txFileUtility::writeFile(fullName, mIndex, mBuffer);
}

void txSerializer::resizeBuffer(const int& maxSize)
{
	int newSize = maxSize > mBufferSize * 2 ? maxSize : mBufferSize * 2;
	char* newBuffer = NULL;
	if (mTraceMemery)
	{
		newBuffer = TRACE_NEW_ARRAY(char, newSize, newBuffer);
	}
	else
	{
		newBuffer = NORMAL_NEW_ARRAY(char, newSize, newBuffer);
	}
	memcpy(newBuffer, mBuffer, mBufferSize);
	if (mTraceMemery)
	{
		TRACE_DELETE_ARRAY(mBuffer);
	}
	else
	{
		NORMAL_DELETE_ARRAY(mBuffer);
	}
	mBuffer = newBuffer;
	mBufferSize = newSize;
}

void txSerializer::createBuffer(const int& bufferSize)
{
	if (mBuffer == NULL)
	{
		mBufferSize = bufferSize;
		if (mTraceMemery)
		{
			mBuffer = TRACE_NEW_ARRAY(char, mBufferSize, mBuffer);
		}
		else
		{
			mBuffer = NORMAL_NEW_ARRAY(char, mBufferSize, mBuffer);
		}
	}
}
