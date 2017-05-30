#ifndef _COMMAND_ROOM_MANAGER_NOTIFY_PLAYER_OFFLINE_H_
#define _COMMAND_ROOM_MANAGER_NOTIFY_PLAYER_OFFLINE_H_

#include "txCommand.h"

class CommandRoomManagerNotifyPlayerOffline : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomManagerNotifyPlayerOffline)
		, mPlayerID(~0)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerID;
};

#endif