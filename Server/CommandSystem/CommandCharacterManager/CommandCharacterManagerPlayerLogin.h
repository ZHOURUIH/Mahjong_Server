﻿#ifndef _COMMAND_CHARACTER_MANAGER_PLAYER_LOGIN_H_
#define _COMMAND_CHARACTER_MANAGER_PLAYER_LOGIN_H_

#include "txCommand.h"

// 玩家登陆,并且发回登陆结果的消息
class CommandCharacterManagerPlayerLogin : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterManagerPlayerLogin)
		, mGUID(~0)
		, mClient(~0)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mGUID;
	std::string mName;
	CLIENT_GUID mClient;
};

#endif