#include "GameLog.h"
#include "GameLogWrap.h"

void GameLogWrap::logError(const string& info, bool delayShow)
{
	GameLog::logError(info, delayShow);
}
void GameLogWrap::logInfo(const string& info, bool delayShow)
{
	GameLog::logInfo(info, delayShow);
}