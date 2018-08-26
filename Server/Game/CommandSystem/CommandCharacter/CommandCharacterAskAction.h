#ifndef _COMMAND_CHARACTER_ASK_ACTION_H_
#define _COMMAND_CHARACTER_ASK_ACTION_H_

#include "txCommand.h"

class MahjongAction;
class CommandCharacterAskAction : public txCommand
{
public:
	virtual void reset()
	{
		mActionList.clear();
	}
	virtual void execute();
public:
	txVector<MahjongAction*> mActionList;
};

#endif