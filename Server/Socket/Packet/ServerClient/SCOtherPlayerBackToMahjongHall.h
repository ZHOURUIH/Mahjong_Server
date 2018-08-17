#ifndef _SC_OTHER_PLAYER_BACK_TO_MAHJONG_HALL_H_
#define _SC_OTHER_PLAYER_BACK_TO_MAHJONG_HALL_H_

#include "Packet.h"

class SCOtherPlayerBackToMahjongHall : public Packet
{
public:
	SCOtherPlayerBackToMahjongHall(PACKET_TYPE type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mOtherPlayerGUID);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("other player guid : %d", mOtherPlayerGUID);
	}
public:
	int mOtherPlayerGUID;
};

#endif