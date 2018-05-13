﻿#ifndef _COMMAND_ROOM_MANAGER_CREATE_ROOM_H_
#define _COMMAND_ROOM_MANAGER_CREATE_ROOM_H_

#include "txCommand.h"

// 创建房间
class Room;
class CommandRoomManagerCreateRoom : public txCommand
{
public:
	virtual void reset()
	{
		mRoomPtr = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	Room** mRoomPtr;
};

#endif