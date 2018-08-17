#ifndef _SC_OTHER_PLAYER_CONTINUE_GAME_H_
#define _SC_OTHER_PLAYER_CONTINUE_GAME_H_

#include "Packet.h"

class SCOtherPlayerContinueGame : public Packet
{
public:
	SCOtherPlayerContinueGame(PACKET_TYPE type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mOtherPlayerGUID);
		pushParam(mBanker);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("other player guid : %d, banker : %s", mOtherPlayerGUID, mBanker ? "true" : "false");
	}
public:
	int mOtherPlayerGUID;
	bool mBanker;
};

#endif