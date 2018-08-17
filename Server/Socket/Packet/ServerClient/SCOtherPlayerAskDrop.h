#ifndef _SC_OTHER_PLAYER_ASK_DROP_H_
#define _SC_OTHER_PLAYER_ASK_DROP_H_

#include "Packet.h"
#include "GameDefine.h"

class SCOtherPlayerAskDrop : public Packet
{
public:
	SCOtherPlayerAskDrop(PACKET_TYPE type)
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
public:
	int mOtherPlayerGUID;
};

#endif