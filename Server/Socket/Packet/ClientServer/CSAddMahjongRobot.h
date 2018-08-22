#ifndef _CS_ADD_MAHJONG_ROBOT_H_
#define _CS_ADD_MAHJONG_ROBOT_H_

#include "Packet.h"

class CSAddMahjongRobot : public Packet
{
public:
	CSAddMahjongRobot(PACKET_TYPE type)
		:Packet(type) {}
	virtual void execute();
	virtual void fillParams(){}
public:
};

#endif