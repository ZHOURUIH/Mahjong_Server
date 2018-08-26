#ifndef _SC_OTHER_PLAYER_GANG_H_
#define _SC_OTHER_PLAYER_GANG_H_

#include "Packet.h"

class SCOtherPlayerGang : public Packet
{
public:
	SCOtherPlayerGang(PACKET_TYPE type)
		:Packet(type) {}
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