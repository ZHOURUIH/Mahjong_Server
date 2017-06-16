#include "ServerDefine.h"
#include "GameDefine.h"

class Character;
class MahjongAction
{
public:
	Character* mActionPlayer;	// 需要操作的玩家
	Character* mDroppedPlayer;	// 打出牌的玩家
	ACTION_TYPE mType;			// 操作类型
	MAHJONG mMah;				// 打出的牌
};

class MahjongActionHu : MahjongAction
{
	std::vector<HU_TYPE> mHuList;       // 胡的所有类型
	MahjongActionHu()
	{
		mType = AT_HU;
	}
};

class MahjongActionPeng : MahjongAction
{
	MahjongActionPeng()
	{
		mType = AT_PENG;
	}
};

class MahjongActionGang : MahjongAction
{
	MahjongActionGang()
	{
		mType = AT_GANG;
	}
};

class MahjongActionPass : MahjongAction
{
	MahjongActionPass()
	{
		mType = AT_PASS;
	}
};