#ifndef _COMMAND_CHARACTER_NOTIFY_BANKER_H_
#define _COMMAND_CHARACTER_NOTIFY_BANKER_H_

#include "txCommand.h"

// 使玩家加入房间
class CommandCharacterNotifyBanker : public txCommand
{
public:
	virtual void reset()
	{
		mBankerID = INVALID_ID;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mBankerID;
};

#endif