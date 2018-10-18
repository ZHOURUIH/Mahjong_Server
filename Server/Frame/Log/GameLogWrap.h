#ifndef _GAME_LOG_WRAP_H_
#define _GAME_LOG_WRAP_H_

// GameLog需要部分的依赖文件,所以直接包含到ServerDefine中会出现错误
// GameLogWrap不包含任何依赖文件,所以可以直接包含到ServerDefine中
class GameLogWrap
{
public:
	static void logError(const std::string& info, bool delayShow);
	static void logInfo(const std::string& info, bool delayShow);
};

#define LOG_ERROR(info)			GameLogWrap::logError(info, false);
#define LOG_ERROR_DELAY(info)	GameLogWrap::logError(info, true);
#define LOG_INFO(info)			GameLogWrap::logInfo(info, false);
#define LOG_INFO_DELAY(info)	GameLogWrap::logInfo(info, true);

#endif