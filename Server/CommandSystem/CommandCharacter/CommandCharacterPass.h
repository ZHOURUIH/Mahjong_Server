#ifndef _COMMAND_CHARACTER_PASS_H_
#define _COMMAND_CHARACTER_PASS_H_

#include "txCommand.h"

class CharacterPlayer;
class CommandCharacterPass : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterPass)
		, mDroppedPlayer(NULL)
		, mMahjong(M_MAX)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CharacterPlayer* mDroppedPlayer;
	MAHJONG mMahjong;
};

#endif