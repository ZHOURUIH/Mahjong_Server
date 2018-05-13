#ifndef _SC_NOTIFY_GET_MAHJONG_H_
#define _SC_NOTIFY_GET_MAHJONG_H_

#include "Packet.h"
#include "GameDefine.h"

class SCNotifyGetMahjong : public Packet
{
public:
	SCNotifyGetMahjong(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mPlayerGUID);
		pushParam(mMahjong);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("player guid : %d, mahjong : %s", mPlayerGUID, MAHJONG_NAME[(int)mMahjong].c_str());
	}
public:
	int mPlayerGUID;
	char mMahjong;
};

#endif