#ifndef _COMMAND_CHARACTER_READY_H_
#define _COMMAND_CHARACTER_READY_H_

#include "txCommand.h"

class CommandCharacterReady : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterReady)
		, mReady(false)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	bool mReady;
};

#endif