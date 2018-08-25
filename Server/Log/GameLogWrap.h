#ifndef _GAME_LOG_WRAP_H_
#define _GAME_LOG_WRAP_H_

// GameLog需要部分的依赖文件,所以直接包含到ServerDefine中会出现错误
// GameLogWrap不包含任何依赖文件,所以可以直接包含到ServerDefine中
class GameLogWrap
{
public:
	static void logError(const std::string& info, bool delay);
	static void logInfo(const std::string& info, bool delay);
};

#define LOG_ERROR(...)												\
{																	\
	char buffer[2048];												\
	SPRINTF(buffer, 2048, __VA_ARGS__);								\
	GameLogWrap::logError(std::string(buffer) + " | " + _FILE_LINE_, false);\
}

#define LOG_ERROR_DELAY(...)												\
{																	\
	char buffer[2048];												\
	SPRINTF(buffer, 2048, __VA_ARGS__);								\
	GameLogWrap::logError(std::string(buffer) + " | " + _FILE_LINE_, true);\
}

#define LOG_INFO(...)						\
{											\
	char buffer[2048];						\
	SPRINTF(buffer, 2048, __VA_ARGS__);		\
	GameLogWrap::logInfo(std::string(buffer), false);	\
}

#define LOG_INFO_DELAY(...)						\
{											\
	char buffer[2048];						\
	SPRINTF(buffer, 2048, __VA_ARGS__);		\
	GameLogWrap::logInfo(std::string(buffer), true);	\
}

#endif