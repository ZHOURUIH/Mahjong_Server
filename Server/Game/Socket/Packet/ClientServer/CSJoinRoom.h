#ifndef _CS_JOIN_ROOM_H_
#define _CS_JOIN_ROOM_H_

#include "Packet.h"

class CSJoinRoom : public Packet
{
public:
	CSJoinRoom(PACKET_TYPE type)
		:Packet(type) {}
	virtual void execute();
	virtual void fillParams()
	{
		pushParam(mRoomID);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("room id : %d", mRoomID);
	}
public:
	int mRoomID;
};

#endif