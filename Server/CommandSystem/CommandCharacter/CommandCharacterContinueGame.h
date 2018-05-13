#ifndef _COMMAND_CHARACTER_CONTINUE_GAME_H_
#define _COMMAND_CHARACTER_CONTINUE_GAME_H_

#include "txCommand.h"

// 玩家继续下一局游戏
class CommandCharacterContinueGame : public txCommand
{
public:
	virtual void reset()
	{
		mContinue = false;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	bool mContinue;
};

#endif