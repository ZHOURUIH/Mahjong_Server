#ifndef _GAME_LOG_H_
#define _GAME_LOG_H_

class GameLog
{
public:
	static void logError(const std::string& info)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		std::cout << "³ÌÐò´íÎó : " << info.c_str() << std::endl;
#elif RUN_PLATFORM == PLATFORM_LINUX
		printf("³ÌÐò´íÎó : %s\n", info.c_str());
#endif
	}
	static void logInfo(const std::string& info)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		std::cout << info.c_str() << std::endl;
#elif RUN_PLATFORM == PLATFORM_LINUX
		printf("%s\n", info.c_str());
#endif
	}
};

#define LOG_ERROR(...)											\
{																\
	char buffer[2048];											\
	SPRINTF(buffer, 2048, __VA_ARGS__);							\
	GameLog::logError(std::string(buffer) + " " + _FILE_LINE_);	\
}

#define LOG_INFO(...)											\
{																\
	char buffer[2048];											\
	SPRINTF(buffer, 2048, __VA_ARGS__);							\
	GameLog::logInfo(std::string(buffer) + " " + _FILE_LINE_);	\
}

#endif