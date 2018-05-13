#ifndef _COMMAND_ROOM_NOTIFY_PLAYER_OFFLINE_H_
#define _COMMAND_ROOM_NOTIFY_PLAYER_OFFLINE_H_

#include "txCommand.h"

class CommandRoomNotifyPlayerOffline : public txCommand
{
public:
	virtual void reset()
	{
		mOfflinePlayer = INVALID_ID;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mOfflinePlayer;
};

#endif