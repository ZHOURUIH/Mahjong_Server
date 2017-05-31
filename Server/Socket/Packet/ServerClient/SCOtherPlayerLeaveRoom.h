﻿#ifndef _SC_OTHER_PLAYER_LEAVE_ROOM_H_
#define _SC_OTHER_PLAYER_LEAVE_ROOM_H_

#include "Packet.h"

class SCOtherPlayerLeaveRoom : public Packet
{
public:
	SCOtherPlayerLeaveRoom(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mPlayerGUID, "guid");
	}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		COMMAND_DEBUG("player id : %d", mPlayerGUID);
	}
public:
	int mPlayerGUID;
};

#endif