#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_GET_HUA_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_GET_HUA_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterNotifyOtherPlayerGetHua : public txCommand
{
public:
	virtual void reset()
	{
		mOtherPlayer = NULL;
		mMahjong = M_MAX;
	}
	virtual void execute();
public:
	CharacterPlayer* mOtherPlayer;
	MAHJONG mMahjong;
};

#endif