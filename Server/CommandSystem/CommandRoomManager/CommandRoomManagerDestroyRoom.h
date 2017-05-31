#ifndef _COMMAND_ROOM_MANAGER_DESTROY_ROOM_H_
#define _COMMAND_ROOM_MANAGER_DESTROY_ROOM_H_

#include "txCommand.h"

// 销毁房间
class CommandRoomManagerDestroyRoom : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomManagerDestroyRoom)
		, mRoomID(INVALID_ID)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	int mRoomID;
};

#endif