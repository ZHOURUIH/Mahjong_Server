#include "CharacterPlayer.h"
#include "CharacterData.h"

void CharacterPlayer::reorderMahjong()
{
	std::sort(mCharacterData->mHandIn.begin(), mCharacterData->mHandIn.end());
}

void CharacterPlayer::getMahjongStart(MAHJONG mah)
{
	mCharacterData->mHandIn.push_back(mah);
}