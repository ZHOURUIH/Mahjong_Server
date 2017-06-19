#ifndef _MAHJONG_ACTION_H_
#define _MAHJONG_ACTION_H_

#include "ServerDefine.h"
#include "GameDefine.h"

class CharacterPlayer;
class MahjongAction
{
public:
	CharacterPlayer* mActionPlayer;	// 需要操作的玩家
	CharacterPlayer* mDroppedPlayer;// 打出牌的玩家
	ACTION_TYPE mType;				// 操作类型
	MAHJONG mMah;					// 打出的牌
	HU_TYPE mHuList[MAX_HU_COUNT];	// 胡的所有类型,只有胡操作才能使用
	MahjongAction(const ACTION_TYPE& type, CharacterPlayer* actionPlayer, CharacterPlayer* droppedPlayer, MAHJONG mah, const std::vector<HU_TYPE>& huList = std::vector<HU_TYPE>())
	{
		mType = type;
		mActionPlayer = actionPlayer;
		mDroppedPlayer = droppedPlayer;
		mMah = mah;
		for (int i = 0; i < MAX_HU_COUNT; ++i)
		{
			if (mType == AT_HU && i < (int)huList.size())
			{
				mHuList[i] = huList[i];
			}
			else
			{
				mHuList[i] = HT_NONE;
			}
		}
	}
};

#endif