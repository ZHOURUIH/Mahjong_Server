#ifndef _SERVER_UTILITY_H_
#define _SERVER_UTILITY_H_

#include "txUtility.h"

#include "GameDefine.h"

class ServerUtility
{
	static txMap<HU_TYPE, int> mMultipleList;
public:
	void init()
	{
		mMultipleList.insert(HT_NORMAL, 1);
		mMultipleList.insert(HT_QINGYISE, 4);
		mMultipleList.insert(HT_QUESE, 1);
		mMultipleList.insert(HT_HUA, 1);
		mMultipleList.insert(HT_GANG, 1);
		mMultipleList.insert(HT_ANGANG, 2);
		mMultipleList.insert(HT_MENQING, 1);
		mMultipleList.insert(HT_DUIDUIHU, 1);
		mMultipleList.insert(HT_ANQIDUI, 4);
		mMultipleList.insert(HT_LONGQIDUI, 8);
		mMultipleList.insert(HT_GANGSHANGHUA, 4);
		mMultipleList.insert(HT_GANGSHANGPAO, 4);
		mMultipleList.insert(HT_HAIDIHUA, 4);
		mMultipleList.insert(HT_HAIDIPAO, 4);
		mMultipleList.insert(HT_TIANHU, 4);
		if (mMultipleList.size() != HT_MAX)
		{
			GAME_ERROR("not all hu type added!");
		}
	}
	static bool isHua(const MAHJONG& mah){ return mah >= M_HUA_CHUN && mah <= M_HUA_JU; }
	static bool canHu(txVector<MAHJONG>& handInMah, const MAHJONG& mah);
	static bool canHu(txVector<MAHJONG>& handInMah);
	static void toMahjongGroup(txVector<MAHJONG>& list, txVector<MahInfo>& group);
	static MAHJONG_HUASE getHuaSe(const MAHJONG& mah);
	static bool isShunzi(txVector<MahInfo>& mahjongList, const int& startIndex);
	static void getHuaseList(txVector<MahInfo>& infoList, txMap<MAHJONG_HUASE, txVector<MAHJONG>> huaseList, bool includeFeng = false);
	static bool canHu(txVector<MahInfo> mahjongList, txVector<txVector<MAHJONG>>& devideList);
	static bool canPeng(txVector<MAHJONG>& handInMah, const MAHJONG& mah);
	static bool canGang(txVector<MAHJONG>& handInMah, const MAHJONG& mah);
	static bool canGang(txVector<MAHJONG>& handInMah);
	static void pengMahjong(txVector<MAHJONG>& handInMah, const MAHJONG& mah);
	static void gangMahjong(txVector<MAHJONG>& handInMah, const MAHJONG& mah);
	static txVector<HU_TYPE> generateHuType(txVector<MAHJONG>& handInMah, const MAHJONG& dropMah, txVector<PengGangInfo*>& gangPengList, bool isSelfGet, bool handInIncludeDrop);
	static bool isQingYiSe(txVector<MAHJONG>& handInMah, txVector<MAHJONG>& pengList, txVector<MAHJONG>& gangList);
	static int getHuMultiple(const HU_TYPE& type)
	{
		txMap<HU_TYPE, int>::iterator iter = mMultipleList.find(type);
		if (iter != mMultipleList.end())
		{
			return iter->second;
		}
		return 0;
	}
};

#endif