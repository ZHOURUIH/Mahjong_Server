#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_ASK_DROP_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_ASK_DROP_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterNotifyOtherPlayerAskDrop : public txCommand
{
public:
	virtual void reset()
	{
		mOtherPlayer = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CharacterPlayer* mOtherPlayer;
};

#endif