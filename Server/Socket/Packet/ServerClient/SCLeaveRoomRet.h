#ifndef _SC_LEAVE_ROOM_RET_H_
#define _SC_LEAVE_ROOM_RET_H_

#include "Packet.h"

class SCLeaveRoomRet : public Packet
{
public:
	SCLeaveRoomRet(PACKET_TYPE type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mResult);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("result : %s", mResult ? "true" : "false");
	}
public:
	bool mResult;
};

#endif