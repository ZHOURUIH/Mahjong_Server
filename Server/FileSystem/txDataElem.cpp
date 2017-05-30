#include "txDataElem.h"

txDataElem::txDataElem()
:mDataPtr(0),
 mSize(0)
{
	memset(k, 0, DATA_NAME_LENGTH);
}

void txDataElem::setSize(const int& size)
{
	TRACE_DELETE_ARRAY(mDataPtr);
	if (size != 0)
	{
		mDataPtr = TRACE_NEW_ARRAY(char, size, mDataPtr);
		memset(mDataPtr, 0, size);
	}
	mSize = size;
}