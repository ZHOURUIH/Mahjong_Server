#ifndef _COMMAND_CHARACTER_GET_START_MAHJONG_H_
#define _COMMAND_CHARACTER_GET_START_MAHJONG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterGetStartMahjong : public txCommand
{
public:
	virtual void reset()
	{
		mMahjong = M_MAX;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	MAHJONG mMahjong;
};

#endif