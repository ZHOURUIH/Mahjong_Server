#ifndef _SC_ASK_DROP_H_
#define _SC_ASK_DROP_H_

#include "Packet.h"
#include "GameDefine.h"

class SCAskDrop : public Packet
{
public:
	SCAskDrop(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams(){}
public:
};

#endif