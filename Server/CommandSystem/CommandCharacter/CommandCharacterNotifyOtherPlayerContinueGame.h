#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_CONTINUE_GAME_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_CONTINUE_GAME_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterNotifyOtherPlayerContinueGame : public txCommand
{
public:
	virtual void reset()
	{
		mOtherPlayer = NULL;
		mContinue = false;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CharacterPlayer* mOtherPlayer;
	bool mContinue;
};

#endif