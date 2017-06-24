#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_GANG_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_GANG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterNotifyOtherPlayerGang : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterNotifyOtherPlayerGang)
		, mOtherPlayer(NULL)
		, mMahjong(M_MAX)
		, mDroppedPlayer(NULL)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CharacterPlayer* mOtherPlayer;
	MAHJONG mMahjong;
	CharacterPlayer* mDroppedPlayer;
};

#endif