#ifndef _COMMAND_ROOM_PLAYER_GET_START_DONE_H_
#define _COMMAND_ROOM_PLAYER_GET_START_DONE_H_

#include "txCommand.h"

// 通知房间已经有玩家准备好了
class CommandRoomPlayerGetStartDone : public txCommand
{
public:
	virtual void reset()
	{
		mPlayerGUID = INVALID_ID;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mPlayerGUID;
};

#endif