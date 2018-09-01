#ifndef _COMMAND_ROOM_NOTIFY_PLAYER_LEAVE_H_
#define _COMMAND_ROOM_NOTIFY_PLAYER_LEAVE_H_

#include "txCommand.h"

// 通知房间有玩家离开
class CharacterPlayer;
class CommandRoomPlayerLeave : public txCommand
{
public:
	virtual void reset()
	{
		mPlayer = NULL;
		mNotifyOtherPlayer = true;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CharacterPlayer* mPlayer;
	bool mNotifyOtherPlayer;
};

#endif