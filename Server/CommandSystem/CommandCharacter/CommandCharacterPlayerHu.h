#ifndef _COMMAND_CHARACTER_PLAYER_HU_H_
#define _COMMAND_CHARACTER_PLAYER_HU_H_

#include "txCommand.h"
#include "GameDefine.h"

class CharacterPlayer;
class CommandCharacterPlayerHu : public txCommand
{
public:
	COMMAND_SERVER_CONSTRUCT(CommandCharacterPlayerHu)
		, mMahjong(M_MAX)
		, mDroppedPlayer(NULL)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	MAHJONG mMahjong;
	std::vector<CharacterPlayer*> mHuPlayerList;
	CharacterPlayer* mDroppedPlayer;
	std::vector<std::vector<HU_TYPE>> mHuList;
};

#endif