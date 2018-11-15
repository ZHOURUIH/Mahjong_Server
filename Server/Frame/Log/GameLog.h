#ifndef _GAME_LOG_H_
#define _GAME_LOG_H_

#include "FrameComponent.h"
#include "ThreadLock.h"

class CustomThread;
class GameLog : public FrameComponent
{
public:
	GameLog(const string& name);
	virtual ~GameLog() { destroy(); }
	virtual void init();
	virtual void update(float elapsedTime);
	void destroy();
	static void logError(const string& info, bool delay = false);
	static void logInfo(const string& info, bool delay = false);
	static void setLog(bool log) { mLog = log; }
protected:
	void log(const string& info);
	void error(const string& info);
	void logDelay(const string& info);
	void errorDelay(const string& info);
	static bool writeLogFile(void* args);
public:
	static volatile atomic<bool> mLog;
	static string mLogFileName;
	static string mErrorFileName;
	CustomThread* mLogThread;
	txVector<string> mLogBuffer;			// 立即显示的提示信息
	txVector<string> mErrorBuffer;			// 立即显示的错误信息
	txVector<string> mLogWriteBuffer;
	txVector<string> mErrorWriteBuffer;
	txVector<string> mLogDelayBuffer;		// 延迟显示的提示信息
	txVector<string> mErrorDelayBuffer;	// 延迟显示的错误信息
	ThreadLock mLogBufferLock;
	ThreadLock mErrorBufferLock;
	ThreadLock mLogDelayLock;
	ThreadLock mErrorDelayLock;
};

#endif