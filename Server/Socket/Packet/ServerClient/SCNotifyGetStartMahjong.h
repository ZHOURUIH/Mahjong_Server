#ifndef _SC_NOTIFY_GET_START_MAHJONG_H_
#define _SC_NOTIFY_GET_START_MAHJONG_H_

#include "Packet.h"
#include "GameDefine.h"

class SCNotifyGetStartMahjong : public Packet
{
public:
	SCNotifyGetStartMahjong(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mPlayerGUID, "player guid");
		pushParam(mMahjong, "mahjong");
	}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("player guid : %d, mahjong : %s", mPlayerGUID, MAHJONG_NAME[mMahjong].c_str());
	}
public:
	int mPlayerGUID;
	char mMahjong;
};

#endif