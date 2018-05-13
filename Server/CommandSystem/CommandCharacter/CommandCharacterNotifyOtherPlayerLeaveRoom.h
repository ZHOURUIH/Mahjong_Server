#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_LEAVE_ROOM_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_LEAVE_ROOM_H_

#include "txCommand.h"

// 通知玩家房间中有其他玩家离开
class CommandCharacterNotifyOtherPlayerLeaveRoom : public txCommand
{
public:
	virtual void reset()
	{
		mLeavePlayerID = INVALID_ID;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mLeavePlayerID;
};

#endif