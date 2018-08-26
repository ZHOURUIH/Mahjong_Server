#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_READY_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_READY_H_

#include "txCommand.h"

// 通知玩家同房间中有其他玩家离线,发送有玩家离线的消息
class CommandCharacterNotifyOtherPlayerReady : public txCommand
{
public:
	virtual void reset()
	{
		mPlayerGUID = INVALID_ID;
		mReady = false;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	bool mReady;
	CHAR_GUID mPlayerGUID;	// 离线玩家的guid
};

#endif