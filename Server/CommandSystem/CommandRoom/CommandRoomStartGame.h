#ifndef _COMMAND_ROOM_START_GAME_H_
#define _COMMAND_ROOM_START_GAME_H_

#include "txCommand.h"

// 通知房间中的所有玩家可以开始游戏(所有玩家已经加载准备完毕)
class CommandRoomStartGame : public txCommand
{
public:
	virtual void reset()
	{
		;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif