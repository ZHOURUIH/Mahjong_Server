#ifndef _COMMAND_CHARACTER_REORDER_MAHJONG_H_
#define _COMMAND_CHARACTER_REORDER_MAHJONG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterReorderMahjong : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterReorderMahjong)
		, mPlayerGUID(INVALID_ID)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerGUID;
};

#endif