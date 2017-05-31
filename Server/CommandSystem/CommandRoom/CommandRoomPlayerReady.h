#ifndef _COMMAND_ROOM_PLAYER_READY_H_
#define _COMMAND_ROOM_PLAYER_READY_H_

#include "txCommand.h"

// 通知房间已经有玩家准备好了(场景加载完成了)
class CommandRoomPlayerReady : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomPlayerReady)
		, mPlayerGUID(INVALID_ID)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerGUID;
};

#endif