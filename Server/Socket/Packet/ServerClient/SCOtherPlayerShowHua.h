#ifndef _SC_OTHER_PLAYER_SHOW_HUA_H_
#define _SC_OTHER_PLAYER_SHOW_HUA_H_

#include "Packet.h"

class SCOtherPlayerShowHua : public Packet
{
public:
	SCOtherPlayerShowHua(const PACKET_TYPE& type)
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
		pushParam(mOtherPlayerGUID);
	}
public:
	char mIndex;
	char mMahjong;
	int mOtherPlayerGUID;
};

#endif