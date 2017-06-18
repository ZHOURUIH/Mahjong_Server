#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_DROP_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_DROP_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterNotifyOtherPlayerDrop : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterNotifyOtherPlayerDrop)
		, mMahjong(M_MAX)
		, mIndex(0)
		, mPlayerGUID(INVALID_ID)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	MAHJONG mMahjong;
	int mIndex;
	CHAR_GUID mPlayerGUID;
};

#endif