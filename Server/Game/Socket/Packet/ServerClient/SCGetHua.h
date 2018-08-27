#ifndef _SC_GET_HUA_H_
#define _SC_GET_HUA_H_

#include "Packet.h"

class SCGetHua : public Packet
{
public:
	SCGetHua(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushParam(mMahjong);
	}
public:
	char mMahjong;
};

#endif