#ifndef _CS_CONFIRM_ACTION_H_
#define _CS_CONFIRM_ACTION_H_

#include "Packet.h"

class CSConfirmAction : public Packet
{
public:
	CSConfirmAction(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams()
	{
		pushParam(mAction);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG(PACKET_EMPTY);
	}
public:
	char mAction;
};

#endif