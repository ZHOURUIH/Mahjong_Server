#ifndef _COMMAND_CHARACTER_PENG_H_
#define _COMMAND_CHARACTER_PENG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterPeng : public txCommand
{
public:
	virtual void reset()
	{
		mMahjong = M_MAX;
		mDroppedPlayer = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	MAHJONG mMahjong;
	CharacterPlayer* mDroppedPlayer;
};

#endif