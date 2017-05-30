#ifndef _SC_HEART_BEAT_RET_H_
#define _SC_HEART_BEAT_RET_H_

#include "Packet.h"

class SCHeartBeatRet : public Packet
{
public:
	SCHeartBeatRet(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute(){}
	virtual void fillParams()
	{
		pushParam(mHeartBeatTimes, "heart beat times");
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("heart beat times : %d", mHeartBeatTimes);
	}
public:
	int mHeartBeatTimes;
};

#endif
