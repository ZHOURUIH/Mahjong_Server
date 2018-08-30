#ifndef _CS_CHECK_NAME_H_
#define _CS_CHECK_NAME_H_

#include "Packet.h"
#include "GameDefine.h"

class CSCheckName : public Packet
{
public:
	CSCheckName(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushArrayParam(mName, MAX_NAME_LENGTH);
	}
	virtual void execute();
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("name : %s", mName);
	}
public:
	char mName[MAX_NAME_LENGTH];
};

#endif