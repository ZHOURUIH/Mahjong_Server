#ifndef _SC_CHECK_ACCOUNT_RET_H_
#define _SC_CHECK_ACCOUNT_RET_H_

#include "Packet.h"

class SCCheckAccountRet : public Packet
{
public:
	SCCheckAccountRet(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushParam(mResult);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("flag : %d", mResult);
	}
public:
	char mResult;			// 为0不存在,1表示已存在
};

#endif