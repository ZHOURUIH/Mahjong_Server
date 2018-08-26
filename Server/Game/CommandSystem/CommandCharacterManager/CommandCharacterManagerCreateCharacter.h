#ifndef _COMMAND_CHARACTER_MANAGER_CREATE_CHARACTER_H_
#define _COMMAND_CHARACTER_MANAGER_CREATE_CHARACTER_H_

#include "txCommand.h"

// 创建角色
class Character;
class CommandCharacterManagerCreateCharacter : public txCommand
{
public:
	virtual void reset()
	{
		mCharGUID = INVALID_ID;
		mClientGUID = INVALID_ID;
		mName = EMPTY_STRING;
		mType = CT_NONE;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mCharGUID;
	CLIENT_GUID mClientGUID;
	std::string mName;
	CHARACTER_TYPE mType;
};

#endif