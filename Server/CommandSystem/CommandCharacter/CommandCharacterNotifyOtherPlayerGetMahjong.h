#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_GET_MAHJONG_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_GET_MAHJONG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterNotifyOtherPlayerGetMahjong : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterNotifyOtherPlayerGetMahjong)
		, mMahjong(M_MAX)
		, mPlayerGUID(INVALID_ID)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	MAHJONG mMahjong;
	CHAR_GUID mPlayerGUID;
};

#endif