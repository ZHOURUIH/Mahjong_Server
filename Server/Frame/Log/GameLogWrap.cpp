#include "GameLog.h"
#include "GameLogWrap.h"

void GameLogWrap::logError(const std::string& info, bool delayShow)
{
	GameLog::logError(info + " | " + _FILE_LINE_, delayShow);
}
void GameLogWrap::logInfo(const std::string& info, bool delayShow)
{
	GameLog::logInfo(info, delayShow);
}