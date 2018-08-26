#ifndef _COMMAND_CHARACTER_ASK_DROP_H_
#define _COMMAND_CHARACTER_ASK_DROP_H_

#include "txCommand.h"

class CommandCharacterAskDrop : public txCommand
{
public:
	virtual void reset()
	{
		;
	}
	virtual void execute();
public:
};

#endif