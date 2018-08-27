#ifndef _COMMAND_CHARACTER_GET_HUA_H_
#define _COMMAND_CHARACTER_GET_HUA_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterGetHua : public txCommand
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