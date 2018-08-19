#include "ServerDefine.h"
#include "GameLog.h"
#include "Utility.h"

volatile std::atomic<bool> GameLog::mLog = true;
std::string GameLog::mLogFileName;
std::string GameLog::mErrorFileName;

GameLog::GameLog(const std::string& name)
	:FrameComponent(name)
{
	mLog = true;
	mLogFileName = SystemUtility::getAvailableResourcePath(LOG_PATH + "log.txt");
	mErrorFileName = SystemUtility::getAvailableResourcePath(LOG_PATH + "error.txt");
	mLogThread = TRACE_NEW(CustomThread, mLogThread, "LogThread");
	mErrorThread = TRACE_NEW(CustomThread, mErrorThread, "ErrorThread");
}

void GameLog::init()
{
	FileUtility::deleteFile(mLogFileName);
	FileUtility::deleteFile(mErrorFileName);
	mLogThread->start(writeLogFile, this);
	mErrorThread->start(writeErrorFile, this);
}

bool GameLog::writeLogFile(void* args)
{
	LOCK(mGameLog->mLogBufferLock);
	// 将日志信息同步到写入列表
	int logCount = mGameLog->mLogBuffer.size();
	for (int i = 0; i < logCount; ++i)
	{
		mGameLog->mLogWriteBuffer.push_back(mGameLog->mLogBuffer[i]);
	}
	mGameLog->mLogBuffer.clear();
	UNLOCK(mGameLog->mLogBufferLock);
	// 写入文件
	int writeCount = mGameLog->mLogWriteBuffer.size();
	for (int i = 0; i < writeCount; ++i)
	{
		FileUtility::writeFile(mLogFileName, mGameLog->mLogWriteBuffer[i] + "\r\n", true);
	}
	mGameLog->mLogWriteBuffer.clear();
	return true;
}

bool GameLog::writeErrorFile(void* args)
{
	LOCK(mGameLog->mErrorBufferLock);
	// 将错误信息同步到写入列表
	int errorCount = mGameLog->mErrorBuffer.size();
	for (int i = 0; i < errorCount; ++i)
	{
		mGameLog->mErrorWriteBuffer.push_back(mGameLog->mErrorBuffer[i]);
	}
	mGameLog->mErrorBuffer.clear();
	UNLOCK(mGameLog->mErrorBufferLock);
	// 写入文件
	int writeCount = mGameLog->mErrorWriteBuffer.size();
	for (int i = 0; i < writeCount; ++i)
	{
		FileUtility::writeFile(mErrorFileName, mGameLog->mErrorWriteBuffer[i] + "\r\n", true);
	}
	mGameLog->mErrorWriteBuffer.clear();
	return true;
}

void GameLog::log(const std::string& info)
{
	LOCK(mLogBufferLock);
	mLogBuffer.push_back(info);
	UNLOCK(mLogBufferLock);
}

void GameLog::error(const std::string& info)
{
	LOCK(mErrorBufferLock);
	mErrorBuffer.push_back(info);
	UNLOCK(mErrorBufferLock);
}

void GameLog::logError(const std::string& info)
{
	std::string fullInfo = std::string(SystemUtility::getTime()) + " | 程序错误 : " + info;
#if RUN_PLATFORM == PLATFORM_WINDOWS
	std::cout << fullInfo << std::endl;
#elif RUN_PLATFORM == PLATFORM_LINUX
	printf("%s\n", fullInfo.c_str());
#endif
	mGameLog->error(fullInfo);
}
void GameLog::logInfo(const std::string& info)
{
	std::string fullInfo = std::string(SystemUtility::getTime()) + " | : " + info;
	if (mLog)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		std::cout << fullInfo << std::endl;
#elif RUN_PLATFORM == PLATFORM_LINUX
		printf("%s\n", fullInfo.c_str());
#endif
	}
	mGameLog->log(fullInfo);
}