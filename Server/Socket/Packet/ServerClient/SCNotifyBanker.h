#ifndef _SC_NOTIFY_BANKER_H_
#define _SC_NOTIFY_BANKER_H_

#include "Packet.h"

class SCNotifyBanker : public Packet
{
public:
	SCNotifyBanker(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mGUID, "guid");
	}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		COMMAND_DEBUG("banker guid : %d", mGUID);
	}
public:
	int mGUID;	// 庄家ID
};

#endif