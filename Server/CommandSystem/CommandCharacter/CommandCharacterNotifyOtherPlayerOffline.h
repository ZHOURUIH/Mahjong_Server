#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_OFFLINE_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_OFFLINE_H_

#include "txCommand.h"

// 通知玩家同房间中有其他玩家离线,发送有玩家离线的消息
class CommandCharacterNotifyOtherPlayerOffline : public txCommand
{
public:
	virtual void reset()
	{
		mPlayerGUID = INVALID_ID;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mPlayerGUID;	// 离线玩家的guid
};

#endif