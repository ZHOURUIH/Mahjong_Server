﻿#ifndef _SC_PLAYER_PENG_H_
#define _SC_PLAYER_PENG_H_

#include "Packet.h"

class SCPlayerPeng : public Packet
{
public:
	SCPlayerPeng(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mDroppedPlayerGUID);
		pushParam(mMahjong);
	}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG(PACKET_EMPTY);
	}
public:
	int mDroppedPlayerGUID;
	char mMahjong;
};

#endif