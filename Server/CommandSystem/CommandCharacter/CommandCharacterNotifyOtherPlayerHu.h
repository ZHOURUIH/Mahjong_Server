#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_HU_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_HU_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterNotifyOtherPlayerHu : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterNotifyOtherPlayerHu)
		, mOtherPlayer(NULL)
		, mDroppedPlayer(NULL)
		, mMahjong(M_MAX)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	MAHJONG mMahjong;
	CharacterPlayer* mOtherPlayer;
	CharacterPlayer* mDroppedPlayer;
	std::vector<HU_TYPE> mHuList;
};

#endif