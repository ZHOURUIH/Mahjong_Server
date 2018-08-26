#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_ASK_ACTION_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_ASK_ACTION_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterNotifyOtherPlayerAskAction : public txCommand
{
public:
	virtual void reset()
	{
		mOtherPlayer = NULL;
	}
	virtual void execute();
public:
	CharacterPlayer* mOtherPlayer;
};

#endif