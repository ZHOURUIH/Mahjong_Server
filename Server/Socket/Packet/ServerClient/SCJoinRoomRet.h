#ifndef _SC_JOIN_ROOM_RET_H_
#define _SC_JOIN_ROOM_RET_H_

#include "Packet.h"

class SCJoinRoomRet : public Packet
{
public:
	SCJoinRoomRet(PACKET_TYPE type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mResult);
		pushParam(mRoomID);
		pushParam(mServerPosition);
		pushParam(mBanker);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("result : %d, room id :%d", (int)mResult, mRoomID);
	}
public:
	char mResult;	// 0表示成功,1表示失败
	int mRoomID;
	char mServerPosition;
	bool mBanker;
};

#endif