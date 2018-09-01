#ifndef _COMMAND_CHARACTER_MAHJONG_POOL_SIZE_H_
#define _COMMAND_CHARACTER_MAHJONG_POOL_SIZE_H_

#include "txCommand.h"

class CommandCharacterMahjongPoolSize : public txCommand
{
public:
	virtual void reset()
	{
		mCount = 0;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	int mCount;
};

#endif