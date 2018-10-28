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
class txCommandReceiver;
struct DelayCommand
{
	float mDelayTime;
	txCommand* mCommand;
	txCommandReceiver* mReceiver;
	DelayCommand(const float& delayTime, txCommand* cmd, txCommandReceiver* receiver)
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
#define NEW_CMD_DELAY(cmd) mCommandSystem->newCmd(cmd, __FILE__, __LINE__, false, true);
#define NEW_CMD_DELAY_INFO(cmd) mCommandSystem->newCmd(cmd, __FILE__, __LINE__, true, true);

// 命令调试信息宏
#if RUN_PLATFORM == PLATFROM_WINDOWS
#define COMMAND_DEBUG(info)															\
{																					\
	std::string cmdName = typeid(*this).name();										\
	cmdName = cmdName.substr(strlen("class "), cmdName.length() - strlen("class "));\
	std::string tempInfo = info;													\
	if (tempInfo.length() > 0)														\
	{																				\
		cmdName += std::string(" : ") + tempInfo;									\
	}																				\
	return cmdName;																	\
}
#elif RUN_PLATFORM == PLATFORM_LINUX
#define COMMAND_DEBUG(info)															\
{																					\
	std::string cmdName = typeid(*this).name();										\
	std::string tempInfo = info;													\
	if (tempInfo.length() > 0)														\
	{																				\
		cmdName += std::string(" : ") + tempInfo;									\
	}																				\
	return cmdName;																	\
}
#endif

#endif
