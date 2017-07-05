#ifndef _COMMAND_CHARACTER_PLAYER_HU_H_
#define _COMMAND_CHARACTER_PLAYER_HU_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterPlayerHu : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterPlayerHu)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	std::vector<CharacterPlayer*> mHuPlayerList;
	std::vector<std::vector<HU_TYPE>> mHuList;
};

#endif