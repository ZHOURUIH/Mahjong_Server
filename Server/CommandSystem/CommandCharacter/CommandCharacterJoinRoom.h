#ifndef _COMMAND_CHARACTER_JOIN_ROOM_H_
#define _COMMAND_CHARACTER_JOIN_ROOM_H_

#include "txCommand.h"

// 使玩家加入房间
class CommandCharacterJoinRoom : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterJoinRoom)
		, mRoomID(~0)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	int mRoomID;
};

#endif