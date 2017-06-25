#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "ServerUtility.h"

void CharacterPlayer::destroy()
{
	int count = mCharacterData->mPengGangList.size();
	for (int i = 0; i < count; ++i)
	{
		TRACE_DELETE(mCharacterData->mPengGangList[i]);
	}
	mCharacterData->mPengGangList.clear();
}

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

// 杠指定牌
void CharacterPlayer::gangMahjong(const MAHJONG& mahjong, CharacterPlayer* dropPlayer)
{
	int pengIndex = -1;
	bool isAlreadyPeng = hasPeng(mahjong, pengIndex);
	if (isAlreadyPeng)
	{
		mCharacterData->mPengGangList[pengIndex]->mType = AT_GANG;
	}
	else
	{
		ServerUtility::gangMahjong(mCharacterData->mHandIn, mahjong);
		addGang(mahjong);
	}
	// 如果是自己摸了一张牌,则需要将自己摸的牌拿出来,如果是其他人打出的牌,则不进行操作
	if (dropPlayer == this)
	{
		int handInCount = mCharacterData->mHandIn.size();
		for (int j = 0; j < handInCount; ++j)
		{
			if (mCharacterData->mHandIn[j] == mahjong)
			{
				mCharacterData->mHandIn.erase(mCharacterData->mHandIn.begin() + j);
				break;
			}
		}
	}
}

void CharacterPlayer::pengMahjong(const MAHJONG& mahjong)
{
	ServerUtility::pengMahjong(mCharacterData->mHandIn, mahjong);
	addPeng(mahjong);
}

void CharacterPlayer::addPeng(const MAHJONG& mahjong)
{
	PengGangInfo* info = TRACE_NEW(PengGangInfo, info);
	info->mMahjong = mahjong;
	info->mType = AT_PENG;
	mCharacterData->mPengGangList.push_back(info);
}

void CharacterPlayer::addGang(const MAHJONG& mahjong)
{
	PengGangInfo* info = TRACE_NEW(PengGangInfo, info);
	info->mMahjong = mahjong;
	info->mType = AT_GANG;
	mCharacterData->mPengGangList.push_back(info);
}

bool CharacterPlayer::hasPeng(const MAHJONG& mahjong, int& pengIndex)
{
	int pengGangCount = mCharacterData->mPengGangList.size();
	for (int i = 0; i < pengGangCount; ++i)
	{
		PengGangInfo* pengGangInfo = mCharacterData->mPengGangList[i];
		if (pengGangInfo->mMahjong == mahjong && pengGangInfo->mType == AT_PENG)
		{
			pengIndex = i;
			return true;
		}
	}
	return false;
}