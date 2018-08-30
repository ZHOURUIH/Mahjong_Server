#ifndef _COMMAND_CHARACTER_START_GAME_H_
#define _COMMAND_CHARACTER_START_GAME_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterStartGame : public txCommand
{
public:
	virtual void reset()
	{
		mDice[0] = 0;
		mDice[1] = 0;
		mBankerPos = -1;
		mPlayerIDList.clear();
		mHandInList.clear();
		mHuaList.clear();
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
	int mBankerPos;
	txVector<CHAR_GUID> mPlayerIDList;
	txVector<txVector<MAHJONG>> mHandInList;
	txVector<txVector<MAHJONG>> mHuaList;
};

#endif