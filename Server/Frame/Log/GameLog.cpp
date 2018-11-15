#include "ServerDefine.h"
#include "GameLog.h"
#include "Utility.h"
#include "txMemoryTrace.h"

volatile atomic<bool> GameLog::mLog(true);
string GameLog::mLogFileName;
string GameLog::mErrorFileName;

GameLog::GameLog(const string& name)
	:FrameComponent(name)
{
	mLog = true;
	mLogFileName = getAvailableResourcePath(LOG_PATH + "log.txt");
	mErrorFileName = getAvailableResourcePath(LOG_PATH + "error.txt");
	TRACE_NEW(CustomThread, mLogThread, "LogThread");
}

void GameLog::init()
{
	deleteFile(mLogFileName);
	deleteFile(mErrorFileName);
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
	FOR(mLogDelayBuffer, int i = 0; i < logCount; ++i)
	{
		if (mLog)
		{
			string fullInfo = string(getTime()) + "\t| : " + mLogDelayBuffer[i];
			print(fullInfo);
		}
		log(mLogDelayBuffer[i]);
	}
	END(mLogDelayBuffer);
	mLogDelayBuffer.clear();
	UNLOCK(mLogDelayLock);

	LOCK(mErrorDelayLock);
	int errorCount = mErrorDelayBuffer.size();
	FOR(mErrorDelayBuffer, int i = 0; i < errorCount; ++i)
	{
		string fullInfo = string(getTime()) + "\t| 程序错误 : " + mErrorDelayBuffer[i];
		print(fullInfo);
		error(mErrorDelayBuffer[i]);
	}
	END(mErrorDelayBuffer);
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
			writeFile(mLogFileName, mGameLog->mLogWriteBuffer[i] + "\r\n", true);
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
			writeFile(mErrorFileName, mGameLog->mErrorWriteBuffer[i] + "\r\n", true);
		}
		mGameLog->mErrorWriteBuffer.clear();
	}
	return true;
}

void GameLog::log(const string& info)
{
	LOCK(mLogBufferLock);
	mLogBuffer.push_back(info);
	UNLOCK(mLogBufferLock);
}

void GameLog::error(const string& info)
{
	LOCK(mErrorBufferLock);
	mErrorBuffer.push_back(info);
	UNLOCK(mErrorBufferLock);
}

void GameLog::logDelay(const string& info)
{
	LOCK(mLogDelayLock);
	mLogDelayBuffer.push_back(info);
	UNLOCK(mLogDelayLock);
}

void GameLog::errorDelay(const string& info)
{
	LOCK(mErrorDelayLock);
	mErrorDelayBuffer.push_back(info);
	UNLOCK(mErrorDelayLock);
}

void GameLog::logError(const string& info, bool delayShow)
{
	if (!delayShow)
	{
		string fullInfo = string(getTime()) + "\t| 程序错误 : " + info;
		print(fullInfo);
		if (mGameLog != NULL)
		{
			mGameLog->error(fullInfo);
		}
	}
	else
	{
		if (mGameLog != NULL)
		{
			mGameLog->errorDelay(info);
		}
	}
}
void GameLog::logInfo(const string& info, bool delayShow)
{
	if (!delayShow)
	{
		string fullInfo = string(getTime()) + "\t| : " + info;
		if (mLog)
		{
			print(fullInfo);
		}
		if (mGameLog != NULL)
		{
			mGameLog->log(fullInfo);
		}
	}
	else
	{
		if (mGameLog != NULL)
		{
			mGameLog->logDelay(info);
		}
	}
}