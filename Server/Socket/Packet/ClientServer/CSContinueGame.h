#ifndef _CS_CONTINUE_GAME_H_
#define _CS_CONTINUE_GAME_H_

#include "Packet.h"

class CSContinueGame : public Packet
{
public:
	CSContinueGame(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams(){}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG(PACKET_EMPTY);
	}
public:
};

#endif