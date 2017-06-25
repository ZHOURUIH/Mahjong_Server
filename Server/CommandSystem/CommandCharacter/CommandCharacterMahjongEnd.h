#ifndef _COMMAND_CHARACTER_MAHJONG_END_H_
#define _COMMAND_CHARACTER_MAHJONG_END_H_

#include "txCommand.h"

class CommandCharacterMahjongEnd : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterMahjongEnd)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
};

#endif