#ifndef _COMMAND_CHARACTER_LEAVE_ROOM_H_
#define _COMMAND_CHARACTER_LEAVE_ROOM_H_

#include "txCommand.h"

// 玩家从房间中移除
class CommandCharacterLeaveRoom : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterLeaveRoom)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
};

#endif