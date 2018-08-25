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
}

void GameLog::init()
{
	FileUtility::deleteFile(mLogFileName);
	FileUtility::deleteFile(mErrorFileName);
	mLogThread->start(writeLogFile, this);
}

void GameLog::destroy()
{
	TRACE_DELETE(mLogThread);
}

void GameLog::update(float elapsedTime)
{
	LOCK(mLogDelayLock);
	int logCount = mLogDelayBuffer.size();
	FOR_STL(mLogDelayBuffer, int i = 0; i < logCount; ++i)
	{
		std::string fullInfo = std::string(SystemUtility::getTime()) + "\t| : " + mLogDelayBuffer[i];
		if (mLog)
		{
#if RUN_PLATFORM == PLATFORM_WINDOWS
			std::cout << fullInfo << std::endl;
#elif RUN_PLATFORM == PLATFORM_LINUX
			printf("%s\n", fullInfo.c_str());
#endif
		}
		log(mLogDelayBuffer[i]);
	}
	END_FOR_STL(mLogDelayBuffer);
	mLogDelayBuffer.clear();
	UNLOCK(mLogDelayLock);

	LOCK(mErrorDelayLock);
	int errorCount = mErrorDelayBuffer.size();
	FOR_STL(mErrorDelayBuffer, int i = 0; i < errorCount; ++i)
	{
		std::string fullInfo = std::string(SystemUtility::getTime()) + "\t| 程序错误 : " + mErrorDelayBuffer[i];
#if RUN_PLATFORM == PLATFORM_WINDOWS
		std::cout << fullInfo << std::endl;
#elif RUN_PLATFORM == PLATFORM_LINUX
		printf("%s\n", fullInfo.c_str());
#endif
		error(mErrorDelayBuffer[i]);
	}
	END_FOR_STL(mErrorDelayBuffer);
	mErrorDelayBuffer.clear();
	UNLOCK(mErrorDelayLock);
}

bool GameLog::writeLogFile(void* args)
{
	// 普通日志
	LOCK(mGameLog->mLogBufferLock);
	// 将日志信息同步到写入列表
	int logCount = mGameLog->mLogBuffer.size();
	if (logCount > 0)
	{
		for (int i = 0; i < logCount; ++i)
		{
			mGameLog->mLogWriteBuffer.push_back(mGameLog->mLogBuffer[i]);
		}
		mGameLog->mLogBuffer.clear();
	}
	UNLOCK(mGameLog->mLogBufferLock);
	// 写入文件
	int writeLogCount = mGameLog->mLogWriteBuffer.size();
	if (writeLogCount > 0)
	{
		for (int i = 0; i < writeLogCount; ++i)
		{
			FileUtility::writeFile(mLogFileName, mGameLog->mLogWriteBuffer[i] + "\r\n", true);
		}
		mGameLog->mLogWriteBuffer.clear();
	}

	// 错误信息
	LOCK(mGameLog->mErrorBufferLock);
	// 将错误信息同步到写入列表
	int errorCount = mGameLog->mErrorBuffer.size();
	if (errorCount > 0)
	{
		for (int i = 0; i < errorCount; ++i)
		{
			mGameLog->mErrorWriteBuffer.push_back(mGameLog->mErrorBuffer[i]);
		}
		mGameLog->mErrorBuffer.clear();
	}
	UNLOCK(mGameLog->mErrorBufferLock);
	// 写入文件
	int writeErrorCount = mGameLog->mErrorWriteBuffer.size();
	if (writeErrorCount > 0)
	{
		for (int i = 0; i < writeErrorCount; ++i)
		{
			FileUtility::writeFile(mErrorFileName, mGameLog->mErrorWriteBuffer[i] + "\r\n", true);
		}
		mGameLog->mErrorWriteBuffer.clear();
	}
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

void GameLog::logDelay(const std::string& info)
{
	LOCK(mLogDelayLock);
	mLogDelayBuffer.push_back(info);
	UNLOCK(mLogDelayLock);
}

void GameLog::errorDelay(const std::string& info)
{
	LOCK(mErrorDelayLock);
	mErrorDelayBuffer.push_back(info);
	UNLOCK(mErrorDelayLock);
}

void GameLog::logError(const std::string& info, bool delay)
{
	if (!delay)
	{
		std::string fullInfo = std::string(SystemUtility::getTime()) + "\t| 程序错误 : " + info;
#if RUN_PLATFORM == PLATFORM_WINDOWS
		std::cout << fullInfo << std::endl;
#elif RUN_PLATFORM == PLATFORM_LINUX
		printf("%s\n", fullInfo.c_str());
#endif
		mGameLog->error(fullInfo);
	}
	else
	{
		mGameLog->errorDelay(info);
	}
}
void GameLog::logInfo(const std::string& info, bool delay)
{
	if (!delay)
	{
		std::string fullInfo = std::string(SystemUtility::getTime()) + "\t| : " + info;
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
	else
	{
		mGameLog->logDelay(info);
	}
}