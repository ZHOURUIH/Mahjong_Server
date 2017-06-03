#ifndef _CS_REGISTER_H_
#define _CS_REGISTER_H_

#include "Packet.h"

class CSRegister : public Packet
{
public:
	CSRegister(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams()
	{
		pushArrayParam(mAccount, 16, "account");
		pushArrayParam(mPassword, 16, "password");
		pushArrayParam(mName, 16, "name");
		pushParam(mHead, "head");
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("account : %s, password : %s, head : %d, name : %s", mAccount, mPassword, mHead, mName);
	}
public:
	char mAccount[16];
	char mPassword[16];
	char mName[16];
	int mHead;
};

#endif