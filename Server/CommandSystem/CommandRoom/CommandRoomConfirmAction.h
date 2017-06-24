#ifndef _COMMAND_ROOM_CONFIRM_ACTION_H_
#define _COMMAND_ROOM_CONFIRM_ACTION_H_

#include "txCommand.h"

class CommandRoomConfirmAction : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomConfirmAction)
		, mPlayerGUID(INVALID_ID)
		, mAction(AT_MAX)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerGUID;
	ACTION_TYPE mAction;
};

#endif