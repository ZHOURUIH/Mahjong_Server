#ifndef _SERVER_UTILITY_H_
#define _SERVER_UTILITY_H_

#include "txUtility.h"

#include "GameDefine.h"

class ServerUtility
{
public:
	void init(){}
	static bool canHu(const std::vector<MAHJONG>& handInMah, const MAHJONG& mah);
	static bool canHu(const std::vector<MAHJONG>& handInMah);
	static void toMahjongGroup(const std::vector<MAHJONG>& list, std::vector<MahInfo>& group);
	static MAHJONG_HUASE getHuaSe(const MAHJONG& mah);
	static bool isShunzi(const std::vector<MahInfo>& mahjongList, const int& startIndex);
	static void getHuaseList(const std::vector<MahInfo>& infoList, std::map<MAHJONG_HUASE, std::vector<MAHJONG>> huaseList, bool includeFeng = false);
	static bool canHu(std::vector<MahInfo> mahjongList, std::vector<std::vector<MAHJONG>>& devideList);
	static bool canPeng(const std::vector<MAHJONG>& handInMah, const MAHJONG& mah);
	static bool canGang(const std::vector<MAHJONG>& handInMah, const MAHJONG& mah);
	static bool canGang(const std::vector<MAHJONG>& handInMah);
	static void pengMahjong(std::vector<MAHJONG>& handInMah, const MAHJONG& mah);
	static void gangMahjong(std::vector<MAHJONG>& handInMah, const MAHJONG& mah);
	static std::vector<HU_TYPE> generateHuType(const std::vector<MAHJONG>& handInMah, const MAHJONG& dropMah, const std::vector<PengGangInfo*>& gangPengList, bool isSelfGet, bool handInIncludeDrop);
	static bool isQingYiSe(const std::vector<MAHJONG>& handInMah, const std::vector<MAHJONG>& pengList, const std::vector<MAHJONG>& gangList);
};

#endif