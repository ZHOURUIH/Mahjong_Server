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
	virtual void fillParams()
	{
		pushArrayParam(mDice, 2, "dice");
	}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		COMMAND_DEBUG("dice : %d, %d", (int)mDice[0], (int)mDice[1]);
	}
	void setDice(char dice0, char dice1)
	{
		mDice[0] = dice0;
		mDice[1] = dice1;
	}
public:
	char mDice[2];
};

#endif