#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_PENG_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_PENG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterNotifyOtherPlayerPeng : public txCommand
{
public:
	virtual void reset()
	{
		mOtherPlayer = NULL;
		mMahjong = M_MAX;
		mDroppedPlayer = NULL;
	}
	virtual void execute();
public:
	CharacterPlayer* mOtherPlayer;
	MAHJONG mMahjong;
	CharacterPlayer* mDroppedPlayer;
};

#endif