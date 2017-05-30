#ifndef _SC_PLAYER_OFFLINE_H_
#define _SC_PLAYER_OFFLINE_H_

#include "Packet.h"

class SCPlayerOffline : public Packet
{
public:
	SCPlayerOffline(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mUserID, "guid");
	}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		COMMAND_DEBUG("player id : %d", mUserID);
	}
public:
	int mUserID;
};

#endif