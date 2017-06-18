#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_REORDER_MAHJONG_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_REORDER_MAHJONG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterNotifyOtherPlayerReorderMahjong : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterNotifyOtherPlayerReorderMahjong)
		, mPlayerGUID(INVALID_ID)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerGUID;
};

#endif