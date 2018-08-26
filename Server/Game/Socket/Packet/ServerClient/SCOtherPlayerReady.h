#ifndef _SC_OTHER_PLAYER_READY_H_
#define _SC_OTHER_PLAYER_READY_H_

#include "Packet.h"

class SCOtherPlayerReady : public Packet
{
public:
	SCOtherPlayerReady(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushParam(mReady);
		pushParam(mPlayerGUID);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("ready : %s, player guid : %d", mReady ? "true" : "false", mPlayerGUID);
	}
public:
	bool mReady;
	int mPlayerGUID;
};

#endif