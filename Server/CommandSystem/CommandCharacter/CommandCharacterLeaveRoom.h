#ifndef _COMMAND_CHARACTER_LEAVE_ROOM_H_
#define _COMMAND_CHARACTER_LEAVE_ROOM_H_

#include "txCommand.h"

// 使玩家离开房间
class CommandCharacterLeaveRoom : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterLeaveRoom)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
};

#endif