#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "ServerUtility.h"

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
	int maxTimes = mCharacterData->mPengGangList.size();
	for (int i = 0; i < maxTimes; ++i)
	{
		// 如果是自己摸了一张已经碰的牌来开杠
		if (mCharacterData->mPengGangList[i]->mMahjong == mahjong
			&& mCharacterData->mPengGangList[i]->mType == AT_PENG)
		{
			mCharacterData->mPengGangList[i]->mType = AT_GANG;
			// 将手里摸的牌拿出
			int handInCount = mCharacterData->mHandIn.size();
			for (int j = 0; j < handInCount; ++j)
			{
				if (mCharacterData->mHandIn[j] == mahjong)
				{
					mCharacterData->mHandIn.erase(mCharacterData->mHandIn.begin() + j);
					break;
				}
			}
			break;
		}
		// 自己手里有三张牌
		else if (mCharacterData->mPengGangList[i]->mMahjong == M_MAX
			&& mCharacterData->mPengGangList[i]->mType == AT_MAX)
		{
			ServerUtility::gangMahjong(mCharacterData->mHandIn, mahjong);
			mCharacterData->mPengGangList[i]->mType = AT_GANG;
			mCharacterData->mPengGangList[i]->mMahjong = mahjong;
			// 自己摸了一张牌后开杠
			if (dropPlayer == this)
			{
				// 将手里摸的牌拿出
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
			// 别人打出一张牌开杠,不做其他操作
			else
			{
				;
			}
			break;
		}
	}
}

void CharacterPlayer::pengMahjong(const MAHJONG& mahjong)
{
	ServerUtility::pengMahjong(mCharacterData->mHandIn, mahjong);
	// 找到一个空的位置,设置为碰的状态
	int maxTimes = mCharacterData->mPengGangList.size();
	for (int i = 0; i < maxTimes; ++i)
	{
		if (mCharacterData->mPengGangList[i]->mType == AT_MAX)
		{
			mCharacterData->mPengGangList[i]->mType = AT_PENG;
			mCharacterData->mPengGangList[i]->mMahjong = mahjong;
			break;
		}
	}
}