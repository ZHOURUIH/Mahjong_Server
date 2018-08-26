#ifndef _CHARACTER_DATA_H_
#define _CHARACTER_DATA_H_

#include "ServerDefine.h"
#include "GameDefine.h"

class CharacterData
{
public:
	CharacterData()
	{
		mGUID = INVALID_ID;
		mHead = 0;
		mMoney = 0;
		mBanker = false;
		mPosition = -1;
		mRoomID = INVALID_INT_ID;
		mReady = false;
	}
	virtual ~CharacterData(){}
public:
	std::string mName;						// 名字
	CHAR_GUID mGUID;						// 唯一ID
	int mHead;								// 头像
	int mMoney;								// 金币数
	bool mBanker;							// 是否为庄家
	int mPosition;							// 玩家的位置,房间中的顺序,客户端中需要依据此顺序,重新排列自己布局的位置
	int mRoomID;							// 玩家当前所在的房间ID
	bool mReady;							// 玩家是否已准备
	txVector<MAHJONG> mHandIn;				// 手里的牌
	txVector<MAHJONG> mDropList;			// 打出的牌
	txVector<PengGangInfo*> mPengGangList;	// 碰或者杠的牌
	txVector<MAHJONG> mHuaList;				// 摸到的花牌
};

#endif