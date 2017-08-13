#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_SHOW_HUA_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_SHOW_HUA_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterNotifyOtherPlayerShowHua : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterNotifyOtherPlayerShowHua)
		, mOtherPlayer(NULL)
		, mMahjong(M_MAX)
		, mIndex(-1)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CharacterPlayer* mOtherPlayer;
	MAHJONG mMahjong;
	int mIndex;
};

#endif