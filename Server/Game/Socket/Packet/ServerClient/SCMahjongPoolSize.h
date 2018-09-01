#ifndef _SC_MAHJONG_POOL_SIZE_H_
#define _SC_MAHJONG_POOL_SIZE_H_

#include "Packet.h"

class SCMahjongPoolSize : public Packet
{
public:
	SCMahjongPoolSize(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushParam(mCount);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("count : %d", mCount);
	}
public:
	char mCount;
};

#endif