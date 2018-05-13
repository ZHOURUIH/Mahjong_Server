#ifndef _GAME_LOG_H_
#define _GAME_LOG_H_

class GameLog
{
public:
	static void logError(const std::string& info);
	static void logInfo(const std::string& info);
};

#define LOG_ERROR(...)												\
{																	\
	char buffer[2048];												\
	SPRINTF(buffer, 2048, __VA_ARGS__);								\
	GameLog::logError(std::string(buffer) + " | " + _FILE_LINE_);	\
}

#define LOG_INFO(...)						\
{											\
	char buffer[2048];						\
	SPRINTF(buffer, 2048, __VA_ARGS__);		\
	GameLog::logInfo(std::string(buffer));	\
}

#endif