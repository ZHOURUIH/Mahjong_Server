#ifndef _COMMAND_ROOM_PLAYER_READY_H_
#define _COMMAND_ROOM_PLAYER_READY_H_

#include "txCommand.h"

class CommandRoomPlayerReady : public txCommand
{
public:
	virtual void reset()
	{
		mPlayerGUID = INVALID_ID;
		mReady = true;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mPlayerGUID;
	bool mReady;
};

#endif