#ifndef _HU_INFO_H_
#define _HU_INFO_H_

#include "GameDefine.h"

class CharacterPlayer;
class HuInfo
{
public:
	CharacterPlayer* mHuPlayer;			// 胡牌的玩家
	CharacterPlayer* mDroppedPlayer;	// 打出这张牌的玩家,如果为自摸,则为胡牌的玩家
	MAHJONG mMahjong;					// 胡牌的麻将
	txVector<HU_TYPE> mHuList;		// 胡牌番型列表
};

#endif