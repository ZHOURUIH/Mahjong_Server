#ifndef _SC_PLAYER_GANG_H_
#define _SC_PLAYER_GANG_H_

#include "Packet.h"

class SCPlayerGang : public Packet
{
public:
	SCPlayerGang(const PACKET_TYPE& type)
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