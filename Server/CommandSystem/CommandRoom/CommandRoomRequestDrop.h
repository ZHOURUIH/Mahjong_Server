#ifndef _COMMAND_ROOM_REQUEST_DROP_H_
#define _COMMAND_ROOM_REQUEST_DROP_H_

#include "txCommand.h"

// 通知房间中的所有玩家可以开始游戏(所有玩家已经加载准备完毕)
class CommandRoomRequestDrop : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandRoomRequestDrop)
		, mPlayerGUID(INVALID_ID)
		, mIndex(0)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	CHAR_GUID mPlayerGUID;
	int mIndex;
};

#endif