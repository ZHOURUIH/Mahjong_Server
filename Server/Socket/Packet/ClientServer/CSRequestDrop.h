#ifndef _CS_REQUEST_DROP_H_
#define _CS_REQUEST_DROP_H_

#include "Packet.h"

class CSRequestDrop : public Packet
{
public:
	CSRequestDrop(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams()
	{
		pushParam(mIndex);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("index : %d", mIndex);
	}
public:
	char mIndex;
};

#endif