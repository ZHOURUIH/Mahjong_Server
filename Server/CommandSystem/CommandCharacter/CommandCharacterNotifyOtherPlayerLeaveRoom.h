#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_LEAVE_ROOM_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_LEAVE_ROOM_H_

#include "txCommand.h"

// 通知玩家房间中有其他玩家离开
class CommandCharacterNotifyOtherPlayerLeaveRoom : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterNotifyOtherPlayerLeaveRoom)
		, mLeavePlayerID(INVALID_ID)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mLeavePlayerID;
};

#endif