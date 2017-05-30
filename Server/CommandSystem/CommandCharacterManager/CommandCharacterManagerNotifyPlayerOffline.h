#ifndef _COMMAND_CHARACTER_MANAGER_NOTIFY_PLAYER_OFFLINE_H_
#define _COMMAND_CHARACTER_MANAGER_NOTIFY_PLAYER_OFFLINE_H_

#include "txCommand.h"

// 通知角色管理器有玩家断线
class CommandCharacterManagerNotifyPlayerOffline : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterManagerNotifyPlayerOffline)
		, mPlayerID(~0)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerID;
};

#endif