#ifndef _COMMAND_ROOM_NOTIFY_PLAYER_LEAVE_H_
#define _COMMAND_ROOM_NOTIFY_PLAYER_LEAVE_H_

#include "txCommand.h"

// 通知房间有玩家离开
class CommandRoomNotifyPlayerLeave : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomNotifyPlayerLeave)
		, mPlayerGUID(INVALID_ID)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerGUID;
};

#endif