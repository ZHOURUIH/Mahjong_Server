#ifndef _SC_ADD_MAHJONG_ROBOT_RET_H_
#define _SC_ADD_MAHJONG_ROBOT_RET_H_

#include "Packet.h"
#include "GameDefine.h"

class SCAddMahjongRobotRet : public Packet
{
public:
	SCAddMahjongRobotRet(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushParam(mResult);
	}
protected:
public:
	bool mResult;
};

#endif