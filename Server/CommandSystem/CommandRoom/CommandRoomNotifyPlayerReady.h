#ifndef _COMMAND_ROOM_NOTIFY_PLAYER_READY_H_
#define _COMMAND_ROOM_NOTIFY_PLAYER_READY_H_

#include "txCommand.h"

// 通知房间已经有玩家准备好了
class CommandRoomNotifyPlayerReady : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomNotifyPlayerReady)
		, mPlayerGUID(INVALID_ID)
		, mReady(false)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerGUID;
	bool mReady;
};

#endif