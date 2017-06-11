#ifndef _COMMAND_CHARACTER_START_GAME_H_
#define _COMMAND_CHARACTER_START_GAME_H_

#include "txCommand.h"

// 向玩家发送开始游戏(进入倒数)的消息
class CommandCharacterStartGame : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterStartGame)
	{}
	COMMAND_DECLARE_FUNCTION;
	void setDice(char dice0, char dice1)
	{
		mDice[0] = dice0;
		mDice[1] = dice1;
	}
public:
	char mDice[2];
};

#endif