#include "CharacterPlayer.h"
#include "CharacterData.h"

void CharacterPlayer::reorderMahjong()
{
	std::sort(mCharacterData->mHandIn.begin(), mCharacterData->mHandIn.end());
}

void CharacterPlayer::getMahjongStart(const MAHJONG& mah)
{
	mCharacterData->mHandIn.push_back(mah);
}

void CharacterPlayer::dropMahjong(const int& index)
{
	MAHJONG mah = mCharacterData->mHandIn[index];
	mCharacterData->mDropList.push_back(mah);
	mCharacterData->mHandIn.erase(mCharacterData->mHandIn.begin() + index);
}