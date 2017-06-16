#ifndef _COMMAND_CHARACTER_GET_START_MAHJONG_H_
#define _COMMAND_CHARACTER_GET_START_MAHJONG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterGetStartMahjong : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterGetStartMahjong)
		, mMahjong(M_MAX)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	MAHJONG mMahjong;
};

#endif