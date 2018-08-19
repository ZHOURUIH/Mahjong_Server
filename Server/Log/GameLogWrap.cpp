#include "GameLog.h"
#include "GameLogWrap.h"

void GameLogWrap::logError(const std::string& info)
{
	GameLog::logError(info);
}
void GameLogWrap::logInfo(const std::string& info)
{
	GameLog::logInfo(info);
}