#ifndef _SC_NOTIFY_REORDER_MAHJONG_H_
#define _SC_NOTIFY_REORDER_MAHJONG_H_

#include "Packet.h"

class SCNotifyReorderMahjong : public Packet
{
public:
	SCNotifyReorderMahjong(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushParam(mPlayerGUID);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("player guid : %d", mPlayerGUID);
	}
public:
	int mPlayerGUID;
};

#endif