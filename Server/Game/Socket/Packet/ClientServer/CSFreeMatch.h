#ifndef _CS_FREE_MATCH_H_
#define _CS_FREE_MATCH_H_

#include "Packet.h"

class CSFreeMatch : public Packet
{
public:
	CSFreeMatch(PACKET_TYPE type)
		:Packet(type) {}
	virtual void execute();
	virtual void fillParams()
	{
		;
	}
public:
};

#endif