#include "GameLog.h"
#include "GameLogWrap.h"

void GameLogWrap::logError(const std::string& info, bool delay)
{
	GameLog::logError(info, delay);
}
void GameLogWrap::logInfo(const std::string& info, bool delay)
{
	GameLog::logInfo(info, delay);
}