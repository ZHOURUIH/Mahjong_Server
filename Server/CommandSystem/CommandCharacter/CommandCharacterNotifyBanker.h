#ifndef _COMMAND_CHARACTER_NOTIFY_BANKER_H_
#define _COMMAND_CHARACTER_NOTIFY_BANKER_H_

#include "txCommand.h"

// 使玩家加入房间
class CommandCharacterNotifyBanker : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterNotifyBanker)
		, mBankerID(INVALID_ID)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mBankerID;
};

#endif