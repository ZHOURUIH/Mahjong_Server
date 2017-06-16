#ifndef _CS_CHECK_ACCOUNT_H_
#define _CS_CHECK_ACCOUNT_H_

#include "Packet.h"

class CSCheckAccount : public Packet
{
public:
	CSCheckAccount(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushArrayParam(mAccount, 16, "check account");
	}
	virtual void execute();
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("account : %s", mAccount);
	}
public:
	char mAccount[16];
};

#endif