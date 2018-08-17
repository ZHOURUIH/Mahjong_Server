#ifndef _CS_CHECK_NAME_H_
#define _CS_CHECK_NAME_H_

#include "Packet.h"

class CSCheckName : public Packet
{
public:
	CSCheckName(PACKET_TYPE type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushArrayParam(mName, 16);
	}
	virtual void execute();
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("name : %s", mName);
	}
public:
	char mName[16];
};

#endif