#ifndef _COMMAND_DEFINE_H_
#define _COMMAND_DEFINE_H_

#include "ServerDefine.h"

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

typedef void(*CommandCallback) (void* user_data, txCommand* cmd);

#define DEBUG_EMPTY "%s", ""

#define CMD_PARAM __FILE__, __LINE__

#define COMMAND_CONSTRUCT(className, baseCommand)									\
	className(const char* file, const int& line, const bool& showInfo = true)		\
	:																				\
	baseCommand(file, line, showInfo)

#define COMMAND_DECLARE_FUNCTION													\
	virtual void execute();															\
	virtual std::string showDebugInfo();

#if RUN_PLATFORM == PLATFROM_WINDOWS

#define COMMAND_DEBUG(...)															\
{																					\
	char strBuf[1024];																\
	SPRINTF(strBuf, 1024, __VA_ARGS__);												\
	std::string cmdName = typeid(*this).name();										\
	cmdName = cmdName.substr(strlen("class "), cmdName.length() - strlen("class "));\
	if (strlen(strBuf) > 0)															\
	{																				\
		cmdName += std::string(" : ") + strBuf;										\
	}																				\
	return cmdName;																	\
}

#elif RUN_PLATFORM == PLATFORM_LINUX

#define COMMAND_DEBUG(...)															\
{																					\
	char strBuf[1024];																\
	SPRINTF(strBuf, 1024, __VA_ARGS__);												\
	std::string cmdName = typeid(*this).name();										\
	if (strlen(strBuf) > 0)															\
	{																				\
		cmdName += std::string(" : ") + strBuf;										\
	}																				\
	return cmdName;																	\
}

#endif

#endif
