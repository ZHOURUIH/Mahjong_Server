#ifndef _COMMAND_ROOM_PLAYER_LEAVE_H_
#define _COMMAND_ROOM_PLAYER_LEAVE_H_

#include "txCommand.h"

// 使玩家离开房间
class CommandRoomPlayerLeave : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomPlayerLeave)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerGUID;
};

#endif