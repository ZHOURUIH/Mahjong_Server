#ifndef _SC_READY_RET_H_
#define _SC_READY_RET_H_

#include "Packet.h"

class SCReadyRet : public Packet
{
public:
	SCReadyRet(PACKET_TYPE type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
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