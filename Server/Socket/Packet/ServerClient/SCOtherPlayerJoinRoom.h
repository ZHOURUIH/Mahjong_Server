#ifndef _SC_OTHER_PLAYER_JOIN_ROOM_H_
#define _SC_OTHER_PLAYER_JOIN_ROOM_H_

#include "Packet.h"

class SCOtherPlayerJoinRoom : public Packet
{
public:
	SCOtherPlayerJoinRoom(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mPlayerGUID);
		pushArrayParam(mName, 16);
		pushParam(mMoney);
		pushParam(mHead);
		pushParam(mPosition);
		pushParam(mReady);
		pushParam(mBanker);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("player guid : %d, name : %s", mPlayerGUID, mName);
	}
	void setName(const std::string& name)
	{
		memcpy(mName, name.c_str(), name.length() < NAME_LENGTH ? name.length() : NAME_LENGTH);
	}
public:
	int mPlayerGUID;
	char mName[16];
	int mMoney;
	int mHead;
	int mPosition;
	bool mReady;
	bool mBanker;
};

#endif