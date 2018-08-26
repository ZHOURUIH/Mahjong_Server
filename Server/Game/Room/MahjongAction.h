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
	txVector<HU_TYPE> mHuList;		// 胡的所有类型,只有胡操作才能使用
public:
	MahjongAction(ACTION_TYPE type, CharacterPlayer* actionPlayer, CharacterPlayer* droppedPlayer, MAHJONG mah, const txVector<HU_TYPE>& huList = txVector<HU_TYPE>())
	{
		mType = type;
		mActionPlayer = actionPlayer;
		mDroppedPlayer = droppedPlayer;
		mMah = mah;
		if (mType == AT_HU)
		{
			mHuList = huList;
		}
	}
};

#endif