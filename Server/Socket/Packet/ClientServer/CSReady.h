#ifndef _CS_READY_H_
#define _CS_READY_H_

#include "Packet.h"

class CSReady : public Packet
{
public:
	CSReady(PACKET_TYPE type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams()
	{
		pushParam(mReady);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("ready : %s", mReady ? "true" : "false");
	}
public:
	bool mReady;
};

#endif