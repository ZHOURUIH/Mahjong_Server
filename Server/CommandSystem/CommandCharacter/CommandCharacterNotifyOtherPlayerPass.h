#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_PASS_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_PASS_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterNotifyOtherPlayerPass : public txCommand
{
public:
	virtual void reset()
	{
		mOtherPlayer = NULL;
		mDroppedPlayer = NULL;
		mMahjong = M_MAX;
	}
	virtual void execute();
public:
	CharacterPlayer* mOtherPlayer;
	CharacterPlayer* mDroppedPlayer;
	MAHJONG mMahjong;
};

#endif