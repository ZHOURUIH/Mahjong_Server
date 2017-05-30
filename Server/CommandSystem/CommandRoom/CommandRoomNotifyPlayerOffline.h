#ifndef _COMMAND_ROOM_NOTIFY_PLAYER_OFFLINE_H_
#define _COMMAND_ROOM_NOTIFY_PLAYER_OFFLINE_H_

#include "txCommand.h"

class CommandRoomNotifyPlayerOffline : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomNotifyPlayerOffline)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mOfflinePlayer;
};

#endif