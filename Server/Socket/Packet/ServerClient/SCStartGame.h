#ifndef _SC_START_GAME_H_
#define _SC_START_GAME_H_

#include "Packet.h"

class SCStartGame : public Packet
{
public:
	SCStartGame(const PACKET_TYPE& type)
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
		COMMAND_DEBUG(DEBUG_EMPTY);
	}
public:
};

#endif