#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_CONTINUE_GAME_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_CONTINUE_GAME_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterNotifyOtherPlayerContinueGame : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterNotifyOtherPlayerContinueGame)
		, mOtherPlayer(NULL)
		, mContinue(false)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CharacterPlayer* mOtherPlayer;
	bool mContinue;
};

#endif