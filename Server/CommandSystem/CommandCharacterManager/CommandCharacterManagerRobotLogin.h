#ifndef _COMMAND_CHARACTER_MANAGER_ROBOT_LOGIN_H_
#define _COMMAND_CHARACTER_MANAGER_ROBOT_LOGIN_H_

#include "txCommand.h"

// 机器人登陆
class CommandCharacterManagerRobotLogin : public txCommand
{
public:
	virtual void reset()
	{
		mGUID = INVALID_ID;
		mName = EMPTY_STRING;
		mMoney = 0;
		mHead = 0;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mGUID;
	std::string mName;
	int mMoney;
	short mHead;
};

#endif