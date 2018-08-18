#ifndef _CS_CONFIRM_ACTION_H_
#define _CS_CONFIRM_ACTION_H_

#include "Packet.h"

class CSConfirmAction : public Packet
{
public:
	CSConfirmAction(PACKET_TYPE type)
		:Packet(type) {}
	virtual void execute();
	virtual void fillParams()
	{
		pushParam(mAction);
	}
public:
	char mAction;
};

#endif