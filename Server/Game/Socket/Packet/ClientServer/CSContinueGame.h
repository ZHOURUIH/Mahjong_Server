#ifndef _CS_CONTINUE_GAME_H_
#define _CS_CONTINUE_GAME_H_

#include "Packet.h"

class CSContinueGame : public Packet
{
public:
	CSContinueGame(PACKET_TYPE type)
		:Packet(type) {}
	virtual void execute();
	virtual void fillParams()
	{
		pushParam(mContinue);
	}
public:
	bool mContinue;
};

#endif