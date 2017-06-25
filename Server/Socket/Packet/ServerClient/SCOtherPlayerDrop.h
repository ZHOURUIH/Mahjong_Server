#ifndef _SC_OTHER_PLAYER_DROP_H_
#define _SC_OTHER_PLAYER_DROP_H_

#include "Packet.h"

class SCOtherPlayerDrop : public Packet
{
public:
	SCOtherPlayerDrop(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mPlayerGUID);
		pushParam(mIndex);
		pushParam(mMahjong);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("player guid : %d, index : %d, mahjong : %d", mPlayerGUID, mIndex, mMahjong);
	}
public:
	int mPlayerGUID;
	int mIndex;
	char mMahjong;
};

#endif