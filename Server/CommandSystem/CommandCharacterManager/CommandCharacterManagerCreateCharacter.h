#ifndef _COMMAND_CHARACTER_MANAGER_CREATE_CHARACTER_H_
#define _COMMAND_CHARACTER_MANAGER_CREATE_CHARACTER_H_

#include "txCommand.h"

// 创建角色
class Character;
class CommandCharacterManagerCreateCharacter : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterManagerCreateCharacter)
		, mCharGUID(~0),
		mClientGUID(~0),
		mType(CT_NONE),
		mResultCharacter(NULL)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mCharGUID;
	CLIENT_GUID mClientGUID;
	std::string mName;
	CHARACTER_TYPE mType;
	Character* mResultCharacter;
};

#endif