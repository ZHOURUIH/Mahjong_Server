#ifndef _SC_BACK_TO_MAHJONG_HALL_H_
#define _SC_BACK_TO_MAHJONG_HALL_H_

#include "Packet.h"

class SCBackToMahjongHallRet : public Packet
{
public:
	SCBackToMahjongHallRet(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams(){}
public:
};

#endif