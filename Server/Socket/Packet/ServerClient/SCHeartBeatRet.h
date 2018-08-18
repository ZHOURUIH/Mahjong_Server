﻿#ifndef _SC_HEART_BEAT_RET_H_
#define _SC_HEART_BEAT_RET_H_

#include "Packet.h"

class SCHeartBeatRet : public Packet
{
public:
	SCHeartBeatRet(PACKET_TYPE type)
		:Packet(type) {}
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
