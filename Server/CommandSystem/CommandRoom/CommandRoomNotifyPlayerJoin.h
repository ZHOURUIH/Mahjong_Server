#ifndef _COMMAND_ROOM_NOTIFY_PLAYER_JOIN_H_
#define _COMMAND_ROOM_NOTIFY_PLAYER_JOIN_H_

#include "txCommand.h"

// 通知房间有玩家离开
class CommandRoomNotifyPlayerJoin : public txCommand
{
public:
	virtual void reset()
	{
		mPlayerGUID = INVALID_ID;
		mResult = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
protected:
	void setResult(const JOIN_ROOM_RESULT& result)
	{
		if (mResult != NULL)
		{
			*mResult = result;
		}
	}
public:
	CHAR_GUID mPlayerGUID;
	JOIN_ROOM_RESULT* mResult;
};

#endif