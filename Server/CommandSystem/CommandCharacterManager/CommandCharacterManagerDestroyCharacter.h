#ifndef _COMMAND_CHARACTER_MANAGER_DESTROY_CHARACTER_H_
#define _COMMAND_CHARACTER_MANAGER_DESTROY_CHARACTER_H_

#include "txCommand.h"

// 销毁角色
class CommandCharacterManagerDestroyCharacter : public txCommand
{
public:
	virtual void reset()
	{
		mGUID = INVALID_ID;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mGUID;
};

#endif