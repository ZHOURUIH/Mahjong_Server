#ifndef _COMMAND_CHARACTER_SHOW_HUA_H_
#define _COMMAND_CHARACTER_SHOW_HUA_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterShowHua : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterShowHua)
		, mIndex(0)
		, mMahjong(M_MAX)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	int mIndex;
	MAHJONG mMahjong;
};

#endif