#ifndef _COMMAND_ROOM_PLAYER_CONTINUE_GAME_H_
#define _COMMAND_ROOM_PLAYER_CONTINUE_GAME_H_

#include "txCommand.h"

// 通知房间有玩家离开
class CharacterPlayer;
class CommandRoomPlayerContinueGame : public txCommand
{
public:
	virtual void reset()
	{
		mPlayer = NULL;
		mContinue = false;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CharacterPlayer* mPlayer;
	bool mContinue;
};

#endif