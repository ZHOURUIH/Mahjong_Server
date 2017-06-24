#ifndef _COMMAND_CHARACTER_HU_H_
#define _COMMAND_CHARACTER_HU_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterHu : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterHu)
		, mMahjong(M_MAX)
		, mDroppedPlayer(NULL)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	MAHJONG mMahjong;
	CharacterPlayer* mDroppedPlayer;
	std::vector<HU_TYPE> mHuList;
};

#endif