#ifndef _COMMAND_ROOM_MANAGER_CREATE_ROOM_H_
#define _COMMAND_ROOM_MANAGER_CREATE_ROOM_H_

#include "txCommand.h"

// 创建房间
class Room;
class CommandRoomManagerCreateRoom : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomManagerCreateRoom)
		, mResultRoom(NULL)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	Room* mResultRoom;
};

#endif