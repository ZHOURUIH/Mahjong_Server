#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_JOIN_ROOM_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_JOIN_ROOM_H_

#include "txCommand.h"

// 通知玩家房间中有其他玩家离开
class Character;
class CommandCharacterNotifyOtherPlayerJoinRoom : public txCommand
{
public:
	virtual void reset()
	{
		mJoinPlayer = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	Character* mJoinPlayer;
};

#endif