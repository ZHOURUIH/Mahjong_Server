#ifndef _SC_SHOW_HUA_H_
#define _SC_SHOW_HUA_H_

#include "Packet.h"

class SCShowHua : public Packet
{
public:
	SCShowHua(const PACKET_TYPE& type)
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
public:
	char mIndex;
	char mMahjong;
};

#endif