#ifndef _CS_REQUEST_ROOM_LIST_H_
#define _CS_REQUEST_ROOM_LIST_H_

#include "Packet.h"

class CSRequestRoomList : public Packet
{
public:
	CSRequestRoomList(PACKET_TYPE type)
		:Packet(type) {}
	virtual void execute();
	virtual void fillParams()
	{
		pushParam(mMinIndex);
		pushParam(mMaxIndex);
	}
public:
	short mMinIndex;
	short mMaxIndex;
};

#endif