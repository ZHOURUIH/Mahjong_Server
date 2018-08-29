#ifndef _COMMAND_CHARACTER_MANAGER_PLAYER_LOGIN_H_
#define _COMMAND_CHARACTER_MANAGER_PLAYER_LOGIN_H_

#include "txCommand.h"

// 玩家登陆,并且发回登陆结果的消息
class CommandCharacterManagerPlayerLogin : public txCommand
{
public:
	virtual void reset()
	{
		mGUID = INVALID_ID;
		mName = EMPTY_STRING;
		mClientID = INVALID_ID;
		mMoney = 0;
		mHead = 0;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mGUID;
	std::string mName;
	CLIENT_GUID mClientID;
	int mMoney;
	short mHead;
};

#endif