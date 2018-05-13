#ifndef _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_REORDER_MAHJONG_H_
#define _COMMAND_CHARACTER_NOTIFY_OTHER_PLAYER_REORDER_MAHJONG_H_

#include "txCommand.h"
#include "GameDefine.h"

class CommandCharacterNotifyOtherPlayerReorderMahjong : public txCommand
{
public:
	virtual void reset()
	{
		mPlayerGUID = INVALID_ID;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	CHAR_GUID mPlayerGUID;
};

#endif