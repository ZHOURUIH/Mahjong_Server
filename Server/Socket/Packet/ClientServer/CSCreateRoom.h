#ifndef _CS_CREATE_ROOM_H_
#define _CS_CREATE_ROOM_H_

#include "Packet.h"

class CSCreateRoom : public Packet
{
public:
	CSCreateRoom(PACKET_TYPE type)
		:Packet(type) {}
	virtual void execute();
	virtual void fillParams(){}
public:
};

#endif