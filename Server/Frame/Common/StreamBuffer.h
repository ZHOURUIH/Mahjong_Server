#ifndef _STREAM_BUFFER_H_
#define _STREAM_BUFFER_H_

#include "ServerDefine.h"

class StreamBuffer
{
protected:
	int mBufferSize;
	char* mBuffer;
	int mDataStart;
	int mDataLength;
public:
	StreamBuffer(int bufferSize);
	~StreamBuffer();
	bool isAvailable(int count);
	void addDataToInputBuffer(const char* data, int count, bool clearIfFull = false);
	void removeDataFromInputBuffer(int count);
	void clearInputBuffer();
	int getDataLength() { return mDataLength; }
	char* getData() { return mBuffer + mDataStart; }
	int getDataStart() { return mDataStart; }
	int getBufferSize() { return mBufferSize - mDataStart; }
protected:
	//-------------------------------------------------------------------------------------------------------------
	void resizeBuffer(int size);
};

#endif