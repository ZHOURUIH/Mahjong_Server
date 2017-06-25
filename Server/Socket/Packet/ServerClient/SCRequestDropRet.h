#ifndef _SC_REQUEST_DROP_RET_H_
#define _SC_REQUEST_DROP_RET_H_

#include "Packet.h"

class SCRequestDropRet : public Packet
{
public:
	SCRequestDropRet(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mIndex);
		pushParam(mMahjong);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("index : %d, mahjong : %d", mIndex, mMahjong);
	}
public:
	char mIndex;
	char mMahjong;
};

#endif