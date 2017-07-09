#ifndef _SERVER_UTILITY_H_
#define _SERVER_UTILITY_H_

#include "txUtility.h"

#include "GameDefine.h"

class ServerUtility
{
	static std::map<HU_TYPE, int> mMultipleList;
public:
	void init()
	{
		mMultipleList.insert(std::make_pair(HT_NORMAL, 1));
		mMultipleList.insert(std::make_pair(HT_QINGYISE, 4));
		mMultipleList.insert(std::make_pair(HT_QUESE, 1));
		mMultipleList.insert(std::make_pair(HT_HUA, 1));
		mMultipleList.insert(std::make_pair(HT_GANG, 1));
		mMultipleList.insert(std::make_pair(HT_ANGANG, 2));
		mMultipleList.insert(std::make_pair(HT_MENQING, 1));
		mMultipleList.insert(std::make_pair(HT_DUIDUIHU, 1));
		mMultipleList.insert(std::make_pair(HT_ANQIDUI, 4));
		mMultipleList.insert(std::make_pair(HT_LONGQIDUI, 8));
		mMultipleList.insert(std::make_pair(HT_GANGSHANGHUA, 4));
		mMultipleList.insert(std::make_pair(HT_GANGSHANGPAO, 4));
		mMultipleList.insert(std::make_pair(HT_HAIDIHUA, 4));
		mMultipleList.insert(std::make_pair(HT_HAIDIPAO, 4));
		mMultipleList.insert(std::make_pair(HT_TIANHU, 4));
		if (mMultipleList.size() != HT_MAX)
		{
			GAME_ERROR("not all hu type added!");
		}
	}
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
	static int getHuMultiple(const HU_TYPE& type)
	{
		std::map<HU_TYPE, int>::iterator iter = mMultipleList.find(type);
		if (iter != mMultipleList.end())
		{
			return iter->second;
		}
		return 0;
	}
};

#endif