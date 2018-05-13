#ifndef _COMMAND_CHARACTER_MANAGER_NOTIFY_PLAYER_OFFLINE_H_
#define _COMMAND_CHARACTER_MANAGER_NOTIFY_PLAYER_OFFLINE_H_

#include "txCommand.h"

// 通知角色管理器有玩家断线
class CommandCharacterManagerNotifyPlayerOffline : public txCommand
{
public:
	virtual void reset()
	{
		mPlayerID = INVALID_ID;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mPlayerID;
};

#endif