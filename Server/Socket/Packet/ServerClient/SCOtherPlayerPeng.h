#ifndef _SC_OTHER_PLAYER_PENG_H_
#define _SC_OTHER_PLAYER_PENG_H_

#include "Packet.h"

class SCOtherPlayerPeng : public Packet
{
public:
	SCOtherPlayerPeng(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mOtherPlayerGUID);
		pushParam(mDroppedPlayerGUID);
		pushParam(mMahjong);
	}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG(PACKET_EMPTY);
	}
public:
	int mOtherPlayerGUID;
	int mDroppedPlayerGUID;
	char mMahjong;
};

#endif