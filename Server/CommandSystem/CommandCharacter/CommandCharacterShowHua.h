#ifndef _COMMAND_CHARACTER_SHOW_HUA_H_
#define _COMMAND_CHARACTER_SHOW_HUA_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterShowHua : public txCommand
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