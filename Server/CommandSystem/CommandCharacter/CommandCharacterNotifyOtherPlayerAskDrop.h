#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_ASK_DROP_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_ASK_DROP_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterNotifyOtherPlayerAskDrop : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterNotifyOtherPlayerAskDrop)
		, mOtherPlayer(NULL)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CharacterPlayer* mOtherPlayer;
};

#endif