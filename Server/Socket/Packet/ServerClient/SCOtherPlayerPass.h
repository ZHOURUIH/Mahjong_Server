#ifndef _SC_OTHER_PLAYER_PASS_H_
#define _SC_OTHER_PLAYER_PASS_H_

#include "Packet.h"

class SCOtherPlayerPass : public Packet
{
public:
	SCOtherPlayerPass(PACKET_TYPE type)
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
public:
	int mOtherPlayerGUID;
	int mDroppedPlayerGUID;
	char mMahjong;
};

#endif