#ifndef _CS_GET_START_MAHJONG_DONE_H_
#define _CS_GET_START_MAHJONG_DONE_H_

#include "Packet.h"

class CSGetStartMahjongDone : public Packet
{
public:
	CSGetStartMahjongDone(PACKET_TYPE type)
		:Packet(type) {}
	virtual void execute();
	virtual void fillParams(){}
public:
};

#endif