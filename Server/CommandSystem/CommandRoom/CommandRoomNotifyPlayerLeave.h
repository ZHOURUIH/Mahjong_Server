#ifndef _COMMAND_ROOM_NOTIFY_PLAYER_LEAVE_H_
#define _COMMAND_ROOM_NOTIFY_PLAYER_LEAVE_H_

#include "txCommand.h"

// 通知房间有玩家离开
class CharacterPlayer;
class CommandRoomNotifyPlayerLeave : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomNotifyPlayerLeave)
		, mPlayer(NULL)
		, mNotifyOtherPlayer(true)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CharacterPlayer* mPlayer;
	bool mNotifyOtherPlayer;
};

#endif