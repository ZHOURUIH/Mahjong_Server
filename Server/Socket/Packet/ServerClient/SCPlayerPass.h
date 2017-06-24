#ifndef _SC_PLAYER_PASS_H_
#define _SC_PLAYER_PASS_H_

#include "Packet.h"

class SCPlayerPass : public Packet
{
public:
	SCPlayerPass(const PACKET_TYPE& type)
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