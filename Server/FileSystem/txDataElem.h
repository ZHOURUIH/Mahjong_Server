#ifndef _TX_DATA_ELEM_H_
#define _TX_DATA_ELEM_H_

#include "txUtility.h"

const int DATA_NAME_LENGTH = 256;
class txDataElem
{
protected:
	char k[DATA_NAME_LENGTH];
	char* mDataPtr;
	int mSize;

public:
	txDataElem();
	virtual ~txDataElem(){ TRACE_DELETE_ARRAY(mDataPtr); }
	void setMem(void* pMem)
	{
		TRACE_DELETE_ARRAY(mDataPtr);
		mDataPtr = (char*)pMem;
	}
	void setSize(const int& size);
	const int& getSize() const { return mSize; }
	void setValue(const char* val)
	{
#ifdef _USE_SAFE_API
		memcpy_s(mDataPtr, mSize, val, mSize);
#else
		memcpy(mDataPtr, val, mSize);
#endif
	}
	char* getValuePtr() const { return mDataPtr; }
	void setKey(const char *key)
	{
#ifdef _USE_SAFE_API
		strcpy_s(k, DATA_NAME_LENGTH, key);
#else
		strcpy(k, key);
#endif
	}
	const char* getKeyPtr() const { return k; }
};

#endif