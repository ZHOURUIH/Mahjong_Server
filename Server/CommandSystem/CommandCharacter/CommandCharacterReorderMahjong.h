#ifndef _COMMAND_CHARACTER_REORDER_MAHJONG_H_
#define _COMMAND_CHARACTER_REORDER_MAHJONG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterReorderMahjong : public txCommand
{
public:
	virtual void reset()
	{
		;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif