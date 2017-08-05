#ifndef _SC_OTHER_PLAYER_CONTINUE_GAME_H_
#define _SC_OTHER_PLAYER_CONTINUE_GAME_H_

#include "Packet.h"

class SCOtherPlayerContinueGame : public Packet
{
public:
	SCOtherPlayerContinueGame(const PACKET_TYPE& type)
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