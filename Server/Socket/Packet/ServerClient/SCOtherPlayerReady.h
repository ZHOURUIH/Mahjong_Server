#ifndef _SC_OTHER_PLAYER_READY_H_
#define _SC_OTHER_PLAYER_READY_H_

#include "Packet.h"

class SCOtherPlayerReady : public Packet
{
public:
	SCOtherPlayerReady(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mReady, "ready");
		pushParam(mPlayerGUID, "player guid");
	}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		COMMAND_DEBUG("ready : %s, player guid : %d", mReady ? "true" : "false", mPlayerGUID);
	}
public:
	bool mReady;
	int mPlayerGUID;
};

#endif