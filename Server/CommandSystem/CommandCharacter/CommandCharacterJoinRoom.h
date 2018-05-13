#ifndef _COMMAND_CHARACTER_JOIN_ROOM_H_
#define _COMMAND_CHARACTER_JOIN_ROOM_H_

#include "txCommand.h"

// 使玩家加入房间
class CommandCharacterJoinRoom : public txCommand
{
public:
	virtual void reset()
	{
		mRoomID = INVALID_INT_ID;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	int mRoomID;
};

#endif