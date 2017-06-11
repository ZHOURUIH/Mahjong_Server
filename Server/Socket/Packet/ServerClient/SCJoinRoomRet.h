#ifndef _SC_JOIN_ROOM_RET_H_
#define _SC_JOIN_ROOM_RET_H_

#include "Packet.h"

class SCJoinRoomRet : public Packet
{
public:
	SCJoinRoomRet(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mResult, "result");
		pushParam(mRoomID, "room id");
		pushParam(mServerPosition, "server position");
		pushParam(mBanker, "banker");
	}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		COMMAND_DEBUG("result : %d, room id :%d", (int)mResult, mRoomID);
	}
public:
	char mResult;	// 0表示成功,1表示失败
	int mRoomID;
	char mServerPosition;
	bool mBanker;
};

#endif