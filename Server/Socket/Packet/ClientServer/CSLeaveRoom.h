#ifndef _CS_LEAVE_ROOM_H_
#define _CS_LEAVE_ROOM_H_

#include "Packet.h"

class CSLeaveRoom : public Packet
{
public:
	CSLeaveRoom(PACKET_TYPE type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams(){}
public:
};

#endif