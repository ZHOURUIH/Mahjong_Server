#ifndef _SC_CREATE_ROOM_RET_H_
#define _SC_CREATE_ROOM_RET_H_

#include "Packet.h"

class SCCreateRoomRet : public Packet
{
public:
	SCCreateRoomRet(PACKET_TYPE type)
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
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("result : %d", (int)mResult);
	}
public:
	char mResult;	// 0表示成功,1表示失败
	int mRoomID;	// 创建的房间ID
};

#endif