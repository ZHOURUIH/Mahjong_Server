#ifndef _DATA_H_
#define _DATA_H_

#include "txSerializedData.h"

#include "DataDefine.h"
#include "GameLog.h"

class Data : public txSerializedData
{
public:
	Data(DATA_TYPE type)
	:
	mType(type)
	{}
	DATA_TYPE getType() { return mType; }
protected:
	DATA_TYPE mType;
};

#endif