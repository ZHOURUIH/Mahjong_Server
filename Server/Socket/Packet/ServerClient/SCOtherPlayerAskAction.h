#ifndef _SC_OTHER_PLAYER_ASK_ACTION_H_
#define _SC_OTHER_PLAYER_ASK_ACTION_H_

#include "Packet.h"
#include "GameDefine.h"
#include "MahjongAction.h"
#include "CharacterPlayer.h"

class SCOtherPlayerAskAction : public Packet
{
public:
	SCOtherPlayerAskAction(PACKET_TYPE type)
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