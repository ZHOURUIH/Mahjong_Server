#ifndef _CS_BACK_TO_MAHJONG_HALL_H_
#define _CS_BACK_TO_MAHJONG_HALL_H_

#include "Packet.h"

class CSBackToMahjongHall : public Packet
{
public:
	CSBackToMahjongHall(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams(){}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG(PACKET_EMPTY);
	}
public:
};

#endif