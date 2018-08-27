#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "Utility.h"

void CharacterPlayer::destroy()
{
	clearMahjong();
}

void CharacterPlayer::reorderMahjong()
{
	std::sort(mCharacterData->mHandIn.begin(), mCharacterData->mHandIn.end());
}

void CharacterPlayer::getMahjong(MAHJONG mah)
{
	mCharacterData->mHandIn.push_back(mah);
}

void CharacterPlayer::dropMahjong(int index)
{
	MAHJONG mah = mCharacterData->mHandIn[index];
	mCharacterData->mDropList.push_back(mah);
	mCharacterData->mHandIn.erase(mCharacterData->mHandIn.begin() + index);
}

void CharacterPlayer::getHua(MAHJONG hua)
{
	mCharacterData->mHuaList.push_back(hua);
}

// 杠指定牌
void CharacterPlayer::gangMahjong(MAHJONG mahjong, CharacterPlayer* dropPlayer)
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
		FOR_STL(mCharacterData->mHandIn, int i = 0; i < handInCount; ++i)
		{
			if (mCharacterData->mHandIn[i] == mahjong)
			{
				mCharacterData->mHandIn.erase(mCharacterData->mHandIn.begin() + i, false);
				break;
			}
		}
		END_FOR_STL(mCharacterData->mHandIn);
	}
}

void CharacterPlayer::pengMahjong(MAHJONG mahjong)
{
	ServerUtility::pengMahjong(mCharacterData->mHandIn, mahjong);
	addPeng(mahjong);
}

void CharacterPlayer::clearMahjong()
{
	mCharacterData->mHandIn.clear();
	mCharacterData->mHuaList.clear();
	mCharacterData->mDropList.clear();
	mCharacterData->mReady = false;
	mCharacterData->mGetStartDone = false;
	int count = mCharacterData->mPengGangList.size();
	FOR_STL(mCharacterData->mPengGangList, int i = 0; i < count; ++i)
	{
		TRACE_DELETE(mCharacterData->mPengGangList[i]);
	}
	END_FOR_STL(mCharacterData->mPengGangList);
	mCharacterData->mPengGangList.clear();
}

void CharacterPlayer::addPeng(MAHJONG mahjong)
{
	PengGangInfo* info = TRACE_NEW(PengGangInfo, info);
	info->mMahjong = mahjong;
	info->mType = AT_PENG;
	mCharacterData->mPengGangList.push_back(info);
}

void CharacterPlayer::addGang(MAHJONG mahjong)
{
	PengGangInfo* info = TRACE_NEW(PengGangInfo, info);
	info->mMahjong = mahjong;
	info->mType = AT_GANG;
	mCharacterData->mPengGangList.push_back(info);
}

bool CharacterPlayer::hasPeng(MAHJONG mahjong, int& pengIndex)
{
	pengIndex = -1;
	int pengGangCount = mCharacterData->mPengGangList.size();
	FOR_STL(mCharacterData->mPengGangList, int i = 0; i < pengGangCount; ++i)
	{
		PengGangInfo* pengGangInfo = mCharacterData->mPengGangList[i];
		if (pengGangInfo->mMahjong == mahjong && pengGangInfo->mType == AT_PENG)
		{
			pengIndex = i;
			break;
		}
	}
	END_FOR_STL(mCharacterData->mPengGangList);
	return pengIndex != -1;
}