#ifndef _COMMAND_CHARACTER_NOTIFY_PLAYER_OFFLINE_H_
#define _COMMAND_CHARACTER_NOTIFY_PLAYER_OFFLINE_H_

#include "txCommand.h"

// 通知玩家同房间中有其他玩家离线,发送有玩家离线的消息
class CommandCharacterNotifyPlayerOffline : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterNotifyPlayerOffline)
		, mPlayerGUID(~0)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerGUID;	// 离线玩家的guid
};

#endif