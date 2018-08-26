#ifndef _COMMAND_CHARACTER_PLAYER_HU_H_
#define _COMMAND_CHARACTER_PLAYER_HU_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterPlayerHu : public txCommand
{
public:
	virtual void reset()
	{
		mHuList.clear();
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	txVector<std::pair<CharacterPlayer*, txVector<HU_TYPE>>> mHuList;
};

#endif