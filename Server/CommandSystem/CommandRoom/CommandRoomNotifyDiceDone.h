#ifndef _COMMAND_ROOM_NOTIFY_DICE_DONE_H_
#define _COMMAND_ROOM_NOTIFY_DICE_DONE_H_

#include "txCommand.h"

class CommandRoomNotifyDiceDone : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomNotifyDiceDone)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerGUID;
};

#endif