#ifndef _WAIT_ACTION_INFO_H_
#define _WAIT_ACTION_INFO_H_

#include "GameDefine.h"

class CharacterPlayer;
class MahjongAction;
class WaitActionInfo
{
public:
	CharacterPlayer* mPlayer;				// 等待确认操作的玩家
	CharacterPlayer* mDroppedPlayer;		// 打出这张牌的玩家
	txVector<MahjongAction*> mActionList;// 玩家可选择的操作
	MAHJONG mMahjong;						// 当前麻将
	MahjongAction* mConfirmedAction;		// 玩家确认选择的一种操作
public:
	WaitActionInfo(CharacterPlayer* player, CharacterPlayer* droppedPlayer, const txVector<MahjongAction*>& actionList, MAHJONG mah)
	{
		mPlayer = player;
		mDroppedPlayer = droppedPlayer;
		mActionList = actionList;
		mMahjong = mah;
		mConfirmedAction = NULL;
	}
};

#endif