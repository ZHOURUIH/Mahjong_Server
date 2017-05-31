#ifndef _SC_OTHER_PLAYER_JOIN_ROOM_H_
#define _SC_OTHER_PLAYER_JOIN_ROOM_H_

#include "Packet.h"

class SCOtherPlayerJoinRoom : public Packet
{
public:
	SCOtherPlayerJoinRoom(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mPlayerID, "guid");
	}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		COMMAND_DEBUG("player id : %d", mPlayerID);
	}
public:
	int mPlayerID;
};

#endif