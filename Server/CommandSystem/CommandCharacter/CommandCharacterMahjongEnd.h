#ifndef _COMMAND_CHARACTER_MAHJONG_END_H_
#define _COMMAND_CHARACTER_MAHJONG_END_H_

#include "txCommand.h"

class CommandCharacterMahjongEnd : public txCommand
{
public:
	virtual void reset()
	{
		;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	txMap<CharacterPlayer*, int> mMoneyDeltaList;
};

#endif