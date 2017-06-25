#ifndef _SC_NOTIFY_MAHJONG_END_H_
#define _SC_NOTIFY_MAHJONG_END_H_

#include "Packet.h"
#include "GameDefine.h"

class SCNotifyMahjongEnd : public Packet
{
public:
	SCNotifyMahjongEnd(const PACKET_TYPE& type)
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