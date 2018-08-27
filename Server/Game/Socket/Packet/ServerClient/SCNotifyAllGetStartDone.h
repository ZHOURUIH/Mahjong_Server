#ifndef _SC_NOTIFY_ALL_GET_START_DONE_H_
#define _SC_NOTIFY_ALL_GET_START_DONE_H_

#include "Packet.h"

class SCNotifyAllGetStartDone : public Packet
{
public:
	SCNotifyAllGetStartDone(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams(){}
public:
};

#endif