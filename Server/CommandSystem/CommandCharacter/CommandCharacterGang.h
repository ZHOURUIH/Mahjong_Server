#ifndef _COMMAND_CHARACTER_GANG_H_
#define _COMMAND_CHARACTER_GANG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterGang : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterGang)
		, mMahjong(M_MAX)
		, mDroppedPlayer(NULL)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	MAHJONG mMahjong;
	CharacterPlayer* mDroppedPlayer;
};

#endif