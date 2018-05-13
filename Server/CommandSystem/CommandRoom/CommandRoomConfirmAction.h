#ifndef _COMMAND_ROOM_CONFIRM_ACTION_H_
#define _COMMAND_ROOM_CONFIRM_ACTION_H_

#include "txCommand.h"

class CommandRoomConfirmAction : public txCommand
{
public:
	virtual void reset()
	{
		mPlayerGUID = INVALID_ID;
		mAction = AT_MAX;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mPlayerGUID;
	ACTION_TYPE mAction;
};

#endif