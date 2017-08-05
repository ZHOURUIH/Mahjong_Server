#ifndef _COMMAND_ROOM_NOTIFY_PLAYER_CONTINUE_GAME_H_
#define _COMMAND_ROOM_NOTIFY_PLAYER_CONTINUE_GAME_H_

#include "txCommand.h"

// 通知房间有玩家离开
class CharacterPlayer;
class CommandRoomNotifyPlayerContinueGame : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomNotifyPlayerContinueGame)
		, mPlayer(NULL)
		, mContinue(false)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CharacterPlayer* mPlayer;
	bool mContinue;
};

#endif