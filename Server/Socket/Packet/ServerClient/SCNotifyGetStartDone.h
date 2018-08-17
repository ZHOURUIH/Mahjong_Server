#ifndef _SC_NOTIFY_GET_START_DONE_H_
#define _SC_NOTIFY_GET_START_DONE_H_

#include "Packet.h"

class SCNotifyGetStartDone : public Packet
{
public:
	SCNotifyGetStartDone(PACKET_TYPE type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams(){}
public:
};

#endif