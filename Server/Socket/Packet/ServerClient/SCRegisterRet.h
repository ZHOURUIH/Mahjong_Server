#ifndef _SC_REGISTER_RET_H_
#define _SC_REGISTER_RET_H_

#include "Packet.h"

class SCRegisterRet : public Packet
{
public:
	SCRegisterRet(const PACKET_TYPE& type)
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
		COMMAND_DEBUG("result : %d", mResult);
	}
public:
	int mResult;	// 0表示成功,-1表示账号已存在,-2表示名字已存在
};

#endif