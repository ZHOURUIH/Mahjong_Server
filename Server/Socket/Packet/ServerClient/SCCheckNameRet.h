#ifndef _SC_CHECK_NAME_RET_H_
#define _SC_CHECK_NAME_RET_H_

#include "Packet.h"

class SCCheckNameRet : public Packet
{
public:
	SCCheckNameRet(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mResult, "result");
	}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("flag : %d", mResult);
	}
public:
	char mResult;			// 为0不存在,1表示已存在
};

#endif