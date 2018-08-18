﻿#ifndef _SC_PLAYER_GANG_H_
#define _SC_PLAYER_GANG_H_

#include "Packet.h"

class SCPlayerGang : public Packet
{
public:
	SCPlayerGang(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushParam(mDroppedPlayerGUID);
		pushParam(mMahjong);
	}
public:
	int mDroppedPlayerGUID;
	char mMahjong;
};

#endif