#ifndef _GAME_LOG_H_
#define _GAME_LOG_H_

#define GAME_ERROR(...)													\
{																		\
	char buffer[2048];													\
	SPRINTF(buffer, 2048, __VA_ARGS__);									\
	txUtility::logError(std::string(buffer) + " " + _FILE_LINE_);		\
}

#define LOG_INFO(...)													\
{																		\
	char buffer[2048];													\
	SPRINTF(buffer, 2048, __VA_ARGS__);									\
	txUtility::logInfo(std::string(buffer) + " " + _FILE_LINE_);		\
}

#define COMMAND_INFO(...)												\
{																		\
	char buffer[2048];													\
	SPRINTF(buffer, 2048, __VA_ARGS__);									\
	txUtility::logInfo(std::string(buffer));							\
}

#endif
