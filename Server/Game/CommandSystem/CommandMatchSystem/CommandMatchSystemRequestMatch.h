#ifndef _COMMAND_MATCH_SYSTEM_REQUEST_MATCH_H_
#define _COMMAND_MATCH_SYSTEM_REQUEST_MATCH_H_

#include "txCommand.h"

class CharacterPlayer;
class CommandMatchSystemRequestMatch : public txCommand
{
public:
	virtual void reset()
	{
		mPlayer = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CharacterPlayer* mPlayer;
};

#endif