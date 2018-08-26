#ifndef _COMMAND_ROOM_NOTIFY_DICE_DONE_H_
#define _COMMAND_ROOM_NOTIFY_DICE_DONE_H_

#include "txCommand.h"

class CommandRoomNotifyDiceDone : public txCommand
{
public:
	virtual void reset()
	{
		mPlayerGUID = INVALID_ID;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mPlayerGUID;
};

#endif