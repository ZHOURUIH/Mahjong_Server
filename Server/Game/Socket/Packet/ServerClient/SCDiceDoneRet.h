#ifndef _SC_DICE_DONE_RET_H_
#define _SC_DICE_DONE_RET_H_

#include "Packet.h"

class SCDiceDoneRet : public Packet
{
public:
	SCDiceDoneRet(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams(){}
public:
};

#endif