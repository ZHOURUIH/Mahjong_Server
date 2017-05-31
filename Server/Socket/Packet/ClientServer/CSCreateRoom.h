#ifndef _CS_CREATE_ROOM_H_
#define _CS_CREATE_ROOM_H_

#include "Packet.h"

class CSCreateRoom : public Packet
{
public:
	CSCreateRoom(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams()
	{
		;
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG(DEBUG_EMPTY);
	}
public:
};

#endif