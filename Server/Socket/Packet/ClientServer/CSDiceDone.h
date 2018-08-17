#ifndef _CS_DICE_DONE_H_
#define _CS_DICE_DONE_H_

#include "Packet.h"

class CSDiceDone : public Packet
{
public:
	CSDiceDone(PACKET_TYPE type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams(){}
public:
};

#endif