#ifndef _SERVER_UTILITY_H_
#define _SERVER_UTILITY_H_

#include "GameDefine.h"

class ServerUtility
{
	static txMap<HU_TYPE, int> mMultipleList;
public:
	void init();
	static bool isHua(MAHJONG mah){ return mah >= M_HUA_CHUN && mah <= M_HUA_JU; }
	static bool canHu(txVector<MAHJONG>& handInMah, MAHJONG mah);
	static bool canHu(txVector<MAHJONG>& handInMah);
	static void toMahjongGroup(txVector<MAHJONG>& list, txVector<MahInfo>& group);
	static MAHJONG_HUASE getHuaSe(MAHJONG mah);
	static bool isShunzi(txVector<MahInfo>& mahjongList, int startIndex);
	static void getHuaseList(txVector<MahInfo>& infoList, txMap<MAHJONG_HUASE, txVector<MAHJONG>> huaseList, bool includeFeng = false);
	static bool canHu(txVector<MahInfo> mahjongList, txVector<txVector<MAHJONG>>& devideList);
	static bool canPeng(txVector<MAHJONG>& handInMah, MAHJONG mah);
	static bool canGang(txVector<MAHJONG>& handInMah, MAHJONG mah);
	static bool canGang(txVector<MAHJONG>& handInMah);
	static void pengMahjong(txVector<MAHJONG>& handInMah, MAHJONG mah);
	static void gangMahjong(txVector<MAHJONG>& handInMah, MAHJONG mah);
	static txVector<HU_TYPE> generateHuType(txVector<MAHJONG>& handInMah, MAHJONG dropMah, txVector<PengGangInfo*>& gangPengList, bool isSelfGet, bool handInIncludeDrop);
	static bool isQingYiSe(txVector<MAHJONG>& handInMah, txVector<MAHJONG>& pengList, txVector<MAHJONG>& gangList);
	static int getHuMultiple(HU_TYPE type)
	{
		return mMultipleList.tryGet(type, 0);
	}
};

#endif