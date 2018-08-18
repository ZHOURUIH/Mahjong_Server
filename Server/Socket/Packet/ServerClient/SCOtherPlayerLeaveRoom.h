﻿#ifndef _SC_OTHER_PLAYER_LEAVE_ROOM_H_
#define _SC_OTHER_PLAYER_LEAVE_ROOM_H_

#include "Packet.h"

class SCOtherPlayerLeaveRoom : public Packet
{
public:
	SCOtherPlayerLeaveRoom(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushParam(mPlayerGUID);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("player id : %d", mPlayerGUID);
	}
public:
	int mPlayerGUID;
};

#endif