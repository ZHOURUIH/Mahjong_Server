#ifndef _COMMAND_CHARACTER_GANG_H_
#define _COMMAND_CHARACTER_GANG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterGang : public txCommand
{
public:
	virtual void reset()
	{
		mMahjong = M_MAX;
		mDroppedPlayer = NULL;
	}
	virtual void execute();
public:
	MAHJONG mMahjong;
	CharacterPlayer* mDroppedPlayer;
};

#endif