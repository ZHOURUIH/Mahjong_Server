#ifndef _CS_BACK_TO_MAHJONG_HALL_H_
#define _CS_BACK_TO_MAHJONG_HALL_H_

#include "Packet.h"

class CSBackToMahjongHall : public Packet
{
public:
	CSBackToMahjongHall(PACKET_TYPE type)
		:Packet(type) {}
	virtual void execute();
	virtual void fillParams(){}
public:
};

#endif