#ifndef _COMMAND_CHARACTER_GET_MAHJONG_H_
#define _COMMAND_CHARACTER_GET_MAHJONG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterGetMahjong : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterGetMahjong)
		, mMahjong(M_MAX)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	MAHJONG mMahjong;
};

#endif