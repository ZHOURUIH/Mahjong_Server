#ifndef _COMMAND_CHARACTER_PENG_H_
#define _COMMAND_CHARACTER_PENG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterPeng : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterPeng)
		, mMahjong(M_MAX)
		, mDroppedPlayer(NULL)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	MAHJONG mMahjong;
	CharacterPlayer* mDroppedPlayer;
};

#endif