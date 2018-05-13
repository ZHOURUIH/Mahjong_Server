#ifndef _COMMAND_ROOM_NOTIFY_PLAYER_READY_H_
#define _COMMAND_ROOM_NOTIFY_PLAYER_READY_H_

#include "txCommand.h"

// 通知房间已经有玩家准备好了
class CommandRoomNotifyPlayerReady : public txCommand
{
public:
	virtual void reset()
	{
		mPlayerGUID = INVALID_ID;
		mReady = false;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mPlayerGUID;
	bool mReady;
};

#endif