#ifndef _COMMAND_CHARACTER_DROP_H_
#define _COMMAND_CHARACTER_DROP_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterDrop : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterDrop)
		, mIndex(0)
		, mMahjong(M_MAX)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	int mIndex;
	MAHJONG mMahjong;
};

#endif