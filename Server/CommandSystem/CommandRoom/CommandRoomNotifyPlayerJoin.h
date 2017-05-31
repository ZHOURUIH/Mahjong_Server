#ifndef _COMMAND_ROOM_NOTIFY_PLAYER_JOIN_H_
#define _COMMAND_ROOM_NOTIFY_PLAYER_JOIN_H_

#include "txCommand.h"

// 通知房间有玩家离开
class CommandRoomNotifyPlayerJoin : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomNotifyPlayerJoin)
		, mPlayerGUID(INVALID_ID)
		, mResult(JRR_NO_ROOM)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerGUID;
	JOIN_ROOM_RESULT mResult;
};

#endif