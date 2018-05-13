#include "ServerDefine.h"
#include "GameLog.h"
#include "Utility.h"

void GameLog::logError(const std::string& info)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	std::cout << txUtility::getTime() << " | 程序错误 : " << info << std::endl;
#elif RUN_PLATFORM == PLATFORM_LINUX
	printf("%s | 程序错误 : %s\n", txUtility::getTime(), info.c_str());
#endif
}
void GameLog::logInfo(const std::string& info)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	std::cout << txUtility::getTime() << " | " << info.c_str() << std::endl;
#elif RUN_PLATFORM == PLATFORM_LINUX
	printf("%s | %s\n", txUtility::getTime(), info.c_str());
#endif
}