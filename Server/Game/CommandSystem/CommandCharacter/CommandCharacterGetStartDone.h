#ifndef _COMMAND_CHARACTER_GET_START_DONE_H_
#define _COMMAND_CHARACTER_GET_START_DONE_H_

#include "txCommand.h"

class CommandCharacterGetStartDone : public txCommand
{
public:
	virtual void reset()
	{}
	virtual void execute();
public:
};

#endif