#ifndef _CS_HEART_BEAT_H_
#define _CS_HEART_BEAT_H_

#include "Packet.h"

class CSHeartBeat : public Packet
{
public:
	CSHeartBeat(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams()
	{
		pushParam(mHeartBeatTimes);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("heart beat times : %d", mHeartBeatTimes);
	}
public:
	int mHeartBeatTimes;
};

#endif
