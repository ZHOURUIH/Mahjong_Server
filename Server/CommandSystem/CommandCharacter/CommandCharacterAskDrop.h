#ifndef _COMMAND_CHARACTER_ASK_DROP_H_
#define _COMMAND_CHARACTER_ASK_DROP_H_

#include "txCommand.h"

// 向玩家发送开始游戏(进入倒数)的消息
class CommandCharacterAskDrop : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterAskDrop)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
};

#endif