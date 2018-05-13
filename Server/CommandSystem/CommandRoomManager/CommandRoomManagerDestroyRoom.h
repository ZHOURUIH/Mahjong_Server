#ifndef _COMMAND_ROOM_MANAGER_DESTROY_ROOM_H_
#define _COMMAND_ROOM_MANAGER_DESTROY_ROOM_H_

#include "txCommand.h"

// 销毁房间
class CommandRoomManagerDestroyRoom : public txCommand
{
public:
	virtual void reset()
	{
		mRoomID = INVALID_INT_ID;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	int mRoomID;
};

#endif