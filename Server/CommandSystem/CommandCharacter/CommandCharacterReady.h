#ifndef _COMMAND_CHARACTER_READY_H_
#define _COMMAND_CHARACTER_READY_H_

#include "txCommand.h"

class CommandCharacterReady : public txCommand
{
public:
	virtual void reset()
	{
		mReady = false;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	bool mReady;
};

#endif