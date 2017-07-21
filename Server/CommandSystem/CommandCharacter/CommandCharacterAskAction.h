#ifndef _COMMAND_CHARACTER_ASK_ACTION_H_
#define _COMMAND_CHARACTER_ASK_ACTION_H_

#include "txCommand.h"

class MahjongAction;
class CommandCharacterAskAction : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterAskAction)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	txVector<MahjongAction*> mActionList;
};

#endif