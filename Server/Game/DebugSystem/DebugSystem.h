#ifndef _DEBUG_SYSTEM_H_
#define _DEBUG_SYSTEM_H_

#include "FrameComponent.h"

const int DEBUG_SYSTEM_CMD = 2;
const std::string ROOM_LIST = "RoomList";
const std::string PLAYER_LIST = "PlayerList";
const std::string MATCH_POOL = "MatchPool";

class txShareMemoryServer;
class DebugSystem : public FrameComponent
{
public:
	DebugSystem(const std::string& name);
	virtual ~DebugSystem() { destroy(); }
	void destroy();
	virtual void init();
	virtual void update(float elapsedTime);
protected:
	void parseCmd(const std::string& param);
protected:
	txShareMemoryServer* mMemoryServer;
	char* mBuffer;
	int mBufferSize;
};

#endif