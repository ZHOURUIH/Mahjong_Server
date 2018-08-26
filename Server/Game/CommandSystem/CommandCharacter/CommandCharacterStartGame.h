#ifndef _COMMAND_CHARACTER_START_GAME_H_
#define _COMMAND_CHARACTER_START_GAME_H_

#include "txCommand.h"
#include "GameDefine.h"

// 向玩家发送开始游戏(进入倒数)的消息
class CommandCharacterStartGame : public txCommand
{
public:
	virtual void reset()
	{
		mDice[0] = 0;
		mDice[1] = 0;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
	void setDice(char dice0, char dice1)
	{
		mDice[0] = dice0;
		mDice[1] = dice1;
	}
public:
	char mDice[2];
	txVector<CHAR_GUID> mPlayerIDList;
	txVector<txVector<MAHJONG>> mHandInList;
	txVector<txVector<MAHJONG>> mHuaList;
};

#endif