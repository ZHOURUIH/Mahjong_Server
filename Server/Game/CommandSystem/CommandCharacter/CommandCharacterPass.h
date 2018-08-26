#ifndef _COMMAND_CHARACTER_PASS_H_
#define _COMMAND_CHARACTER_PASS_H_

#include "txCommand.h"

class CharacterPlayer;
class CommandCharacterPass : public txCommand
{
public:
	virtual void reset()
	{
		mDroppedPlayer = NULL;
		mMahjong = M_MAX;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CharacterPlayer* mDroppedPlayer;
	MAHJONG mMahjong;
};

#endif