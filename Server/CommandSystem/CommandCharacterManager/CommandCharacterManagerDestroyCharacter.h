#ifndef _COMMAND_CHARACTER_MANAGER_DESTROY_CHARACTER_H_
#define _COMMAND_CHARACTER_MANAGER_DESTROY_CHARACTER_H_

#include "txCommand.h"

// 销毁角色
class CommandCharacterManagerDestroyCharacter : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterManagerDestroyCharacter)
		, mGUID(INVALID_ID)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mGUID;
};

#endif