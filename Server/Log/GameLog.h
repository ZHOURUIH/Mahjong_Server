#ifndef _GAME_LOG_H_
#define _GAME_LOG_H_

#include "FrameComponent.h"
#include "ThreadLock.h"

class CustomThread;
class GameLog : public FrameComponent
{
public:
	GameLog(const std::string& name);
	virtual ~GameLog() { destroy(); }
	virtual void init();
	virtual void update(float elapsedTime) {}
	void destroy();
	static void logError(const std::string& info);
	static void logInfo(const std::string& info);
	static void setLog(bool log) { mLog = log; }
protected:
	void log(const std::string& info);
	void error(const std::string& info);
	static bool writeLogFile(void* args);
public:
	static volatile std::atomic<bool> mLog;
	static std::string mLogFileName;
	static std::string mErrorFileName;
	CustomThread* mLogThread;
	txVector<std::string> mLogBuffer;
	txVector<std::string> mErrorBuffer;
	txVector<std::string> mLogWriteBuffer;
	txVector<std::string> mErrorWriteBuffer;
	ThreadLock mLogBufferLock;
	ThreadLock mErrorBufferLock;
};

#endif