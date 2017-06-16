#ifndef _SC_DICE_DONE_RET_H_
#define _SC_DICE_DONE_RET_H_

#include "Packet.h"

class SCDiceDoneRet : public Packet
{
public:
	SCDiceDoneRet(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams(){}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG(PACKET_EMPTY);
	}
public:
};

#endif