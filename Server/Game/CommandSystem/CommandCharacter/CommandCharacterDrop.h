#ifndef _COMMAND_CHARACTER_DROP_H_
#define _COMMAND_CHARACTER_DROP_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterDrop : public txCommand
{
public:
	virtual void reset()
	{
		mIndex = 0;
		mMahjong = M_MAX;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	int mIndex;
	MAHJONG mMahjong;
};

#endif