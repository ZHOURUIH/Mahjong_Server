#ifndef _COMMAND_DEFINE_H_
#define _COMMAND_DEFINE_H_

#include "ServerDefine.h"

enum EXECUTE_STATE
{
	ES_NOT_EXECUTE,
	ES_EXECUTING,
	ES_EXECUTED,
};

class txCommand;
class CommandReceiver;
struct DelayCommand
{
	float mDelayTime;
	txCommand* mCommand;
	CommandReceiver* mReceiver;
	DelayCommand(const float& delayTime, txCommand* cmd, CommandReceiver* receiver)
		:
		mDelayTime(delayTime),
		mCommand(cmd),
		mReceiver(receiver)
	{}
};

typedef void(*CommandCallback) (txCommand* cmd, void* user_data);

#define CMD_CAST static_cast
#define CMD_PARAM __FILE__, __LINE__

#define NEW_CMD(cmd) mCommandSystem->newCmd(cmd, __FILE__, __LINE__, false, false);
#define NEW_CMD_INFO(cmd) mCommandSystem->newCmd(cmd, __FILE__, __LINE__, true, false);
#define NEW_CMD_SHOW(cmd, showInfo) mCommandSystem->newCmd(cmd, __FILE__, __LINE__, showInfo, false);
#define NEW_CMD_DELAY(cmd) mCommandSystem->newCmd(cmd, __FILE__, __LINE__, false, true);
#define NEW_CMD_DELAY_INFO(cmd) mCommandSystem->newCmd(cmd, __FILE__, __LINE__, true, true);

// 命令调试信息宏
#if RUN_PLATFORM == PLATFROM_WINDOWS
#define COMMAND_DEBUG(info)															\
{																					\
	string cmdName = typeid(*this).name();										\
	cmdName = cmdName.substr(strlen("class "), cmdName.length() - strlen("class "));\
	string tempInfo = info;													\
	if (tempInfo.length() > 0)														\
	{																				\
		cmdName += string(" : ") + tempInfo;									\
	}																				\
	return cmdName;																	\
}
#elif RUN_PLATFORM == PLATFORM_LINUX
#define COMMAND_DEBUG(info)															\
{																					\
	string cmdName = typeid(*this).name();										\
	string tempInfo = info;													\
	if (tempInfo.length() > 0)														\
	{																				\
		cmdName += string(" : ") + tempInfo;									\
	}																				\
	return cmdName;																	\
}
#endif

#endif
