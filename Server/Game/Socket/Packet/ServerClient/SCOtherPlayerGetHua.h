#ifndef _SC_OTHER_PLAYER_GET_HUA_H_
#define _SC_OTHER_PLAYER_GET_HUA_H_

#include "Packet.h"

class SCOtherPlayerGetHua : public Packet
{
public:
	SCOtherPlayerGetHua(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushParam(mMahjong);
		pushParam(mOtherPlayerGUID);
	}
public:
	char mMahjong;
	int mOtherPlayerGUID;
};

#endif