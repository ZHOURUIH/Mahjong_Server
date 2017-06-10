#ifndef _CS_LEAVE_ROOM_H_
#define _CS_LEAVE_ROOM_H_

#include "Packet.h"

class CSLeaveRoom : public Packet
{
public:
	CSLeaveRoom(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams()
	{}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG(PACKET_EMPTY);
	}
public:
};

#endif