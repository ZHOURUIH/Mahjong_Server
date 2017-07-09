#include "ServerUtility.h"

std::map<HU_TYPE, int> ServerUtility::mMultipleList;

// handInMah必须是从小到大的有序数组
bool ServerUtility::canHu(const std::vector<MAHJONG>& handInMah, const MAHJONG& mah)
{
	//复制一份列表
	std::vector<MAHJONG> temp = handInMah;
	// 然后别人打出的牌加入其中
	temp.push_back(mah);
	return canHu(temp);
}

bool ServerUtility::canHu(const std::vector<MAHJONG>& handInMah)
{
	std::vector<std::vector<MAHJONG>> devide;
	std::vector<MahInfo> group;
	toMahjongGroup(handInMah, group);
	return canHu(group, devide);
}

void ServerUtility::toMahjongGroup(const std::vector<MAHJONG>& list, std::vector<MahInfo>& group)
{
	std::map<MAHJONG, MahInfo> groupMap;
	int listSize = list.size();
	for (int i = 0; i < listSize; ++i)
	{
		const MAHJONG& mah = list[i];
		if (groupMap.find(mah) != groupMap.end())
		{
			groupMap[mah].mCount += 1;
		}
		else
		{
			MahInfo info;
			info.mMah = mah;
			info.mCount = 1;
			groupMap.insert(std::make_pair(mah, info));
		}
	}
	std::map<MAHJONG, MahInfo>::iterator iter = groupMap.begin();
	std::map<MAHJONG, MahInfo>::iterator iterEnd = groupMap.end();
	for (; iter != iterEnd; ++iter)
	{
		group.push_back(iter->second);
	}
}

// 得到指定牌的花色
MAHJONG_HUASE ServerUtility::getHuaSe(const MAHJONG& mah)
{
	if (mah >= M_FENG_DONG && mah <= M_FENG_BAI)
	{
		return MH_FENG;
	}
	else
	{
		int huaseIndex = (mah - M_TONG1) / 9;
		return MAHJONG_HUASE(MH_TONG + huaseIndex);
	}
}

bool ServerUtility::isShunzi(const std::vector<MahInfo>& mahjongList, const int& startIndex)
{
	if ((int)mahjongList.size() <= startIndex + 2)
	{
		return false;
	}
	if (getHuaSe(mahjongList[startIndex].mMah) == MH_FENG ||
		getHuaSe(mahjongList[startIndex + 1].mMah) == MH_FENG ||
		getHuaSe(mahjongList[startIndex + 2].mMah) == MH_FENG)
	{
		return false;
	}
	// 必须是同花色的
	MAHJONG_HUASE huase0 = getHuaSe(mahjongList[startIndex].mMah);
	MAHJONG_HUASE huase1 = getHuaSe(mahjongList[startIndex + 1].mMah);
	MAHJONG_HUASE huase2 = getHuaSe(mahjongList[startIndex + 2].mMah);
	if (huase0 != huase1 || huase1 != huase2)
	{
		return false;
	}
	if (mahjongList[startIndex].mMah + 1 != mahjongList[startIndex + 1].mMah ||
		mahjongList[startIndex + 1].mMah + 1 != mahjongList[startIndex + 2].mMah)
	{
		return false;
	}
	return true;
}

void ServerUtility::getHuaseList(const std::vector<MahInfo>& infoList, std::map<MAHJONG_HUASE, std::vector<MAHJONG>> huaseList, bool includeFeng)
{
	int size = infoList.size();
	for (int i = 0; i < size; ++i)
	{
		MAHJONG_HUASE huase = getHuaSe(infoList[i].mMah);
		if (huaseList.find(huase) == huaseList.end())
		{
			if (includeFeng || huase != MH_FENG)
			{
				std::vector<MAHJONG> temp;
				temp.push_back(infoList[i].mMah);
				huaseList.insert(std::make_pair(huase, temp));
			}
		}
		else
		{
			huaseList[huase].push_back(infoList[i].mMah);
		}
	}
}

bool ServerUtility::canHu(std::vector<MahInfo> mahjongList, std::vector<std::vector<MAHJONG>>& devideList)
{
	// 必须至少缺一色牌才能胡,
	std::map<MAHJONG_HUASE, std::vector<MAHJONG>> huaseList;
	getHuaseList(mahjongList, huaseList);
	if (huaseList.size() > 2)
	{
		return false;
	}
	int allCount = 0;
	int size = mahjongList.size();
	for (int i = 0; i < size; ++i)
	{
		allCount += mahjongList[i].mCount;
	}
	if (allCount > 2)
	{
		// 取出所有可能的顺子和三个相同的,逐一判断是否可胡
		int index = 0;
		while (true)
		{
			// 没找到顺子,则退出循环
			if (index >= (int)mahjongList.size())
			{
				break;
			}
			// 判断是否有三个相同的
			if (mahjongList[index].mCount >= 3)
			{
				// 先备份列表
				std::vector<MahInfo> beforeList = mahjongList;
				// 移除三个相同的,判断去除后是否可胡
				MAHJONG curMah = mahjongList[index].mMah;
				mahjongList[index].mCount -= 3;
				if (mahjongList[index].mCount == 0)
				{
					mahjongList.erase(mahjongList.begin() + index);
				}
				if (canHu(mahjongList, devideList))
				{
					std::vector<MAHJONG> temp;
					temp.push_back(curMah);
					temp.push_back(curMah);
					temp.push_back(curMah);
					devideList.push_back(temp);
					return true;
				}
				// 不能胡则还原回去
				else
				{
					mahjongList = beforeList;
				}
			}
			// 判断是否有顺子
			if (isShunzi(mahjongList, index))
			{
				// 先备份列表
				std::vector<MahInfo> beforeList = mahjongList;
				MAHJONG mah0 = mahjongList[index].mMah;
				MAHJONG mah1 = mahjongList[index + 1].mMah;
				MAHJONG mah2 = mahjongList[index + 2].mMah;
				// 移除该顺子,判断去除后是否可胡,需要从后往前判断,避免移除后影响下标
				mahjongList[index + 2].mCount -= 1;
				if (mahjongList[index + 2].mCount == 0)
				{
					mahjongList.erase(mahjongList.begin() + index + 2);
				}
				mahjongList[index + 1].mCount -= 1;
				if (mahjongList[index + 1].mCount == 0)
				{
					mahjongList.erase(mahjongList.begin() + index + 1);
				}
				mahjongList[index].mCount -= 1;
				if (mahjongList[index].mCount == 0)
				{
					mahjongList.erase(mahjongList.begin() + index);
				}
				// 如果可以胡,则直接返回true
				if (canHu(mahjongList, devideList))
				{
					std::vector<MAHJONG> temp;
					temp.push_back(mah0);
					temp.push_back(mah1);
					temp.push_back(mah2);
					devideList.push_back(temp);
					return true;
				}
				// 不能胡,则需要将顺子还原到列表中,然后继续寻找下一个顺子
				else
				{
					mahjongList = beforeList;
				}
			}
			// 判断三个相同和顺子后都不能胡,就继续往后找
			++index;
		}
		// 遍历到最后一个顺子都没发现可以胡,则不能胡
		return false;
	}
	// 判断最后剩的两张牌是否为将牌
	else
	{
		bool ret = (mahjongList.size() == 1 && mahjongList[0].mCount == 2);
		if (ret)
		{
			std::vector<MAHJONG> temp;
			temp.push_back(mahjongList[0].mMah);
			temp.push_back(mahjongList[0].mMah);
			devideList.push_back(temp);
		}
		return ret;
	}
}

bool ServerUtility::canPeng(const std::vector<MAHJONG>& handInMah, const MAHJONG& mah)
{
	std::vector<MahInfo> infoList;
	toMahjongGroup(handInMah, infoList);
	int count = infoList.size();
	for (int i = 0; i < count; ++i)
	{
		if (infoList[i].mCount >= 2 && infoList[i].mMah == mah)
		{
			return true;
		}
	}
	return false;
}

bool ServerUtility::canGang(const std::vector<MAHJONG>& handInMah, const MAHJONG& mah)
{
	std::vector<MahInfo> infoList;
	toMahjongGroup(handInMah, infoList);
	int count = infoList.size();
	for (int i = 0; i < count; ++i)
	{
		if (infoList[i].mCount == MAX_SINGLE_COUNT - 1 && infoList[i].mMah == mah)
		{
			return true;
		}
	}
	return false;
}

bool ServerUtility::canGang(const std::vector<MAHJONG>& handInMah)
{
	std::vector<MahInfo> infoList;
	toMahjongGroup(handInMah, infoList);
	int count = infoList.size();
	for (int i = 0; i < count; ++i)
	{
		if (infoList[i].mCount == MAX_SINGLE_COUNT)
		{
			return true;
		}
	}
	return false;
}

void ServerUtility::pengMahjong(std::vector<MAHJONG>& handInMah, const MAHJONG& mah)
{
	// 碰的前提是之前检测过可以碰
	int mahCount = handInMah.size();
	for (int i = 0; i < mahCount - 1; ++i)
	{
		if (handInMah[i] == mah && handInMah[i + 1] == mah)
		{
			// 从后往前删除
			handInMah.erase(handInMah.begin() + i + 1);
			handInMah.erase(handInMah.begin() + i);
			break;
		}
	}
}

void ServerUtility::gangMahjong(std::vector<MAHJONG>& handInMah, const MAHJONG& mah)
{
	// 杠的前提是之前检测过可以杠
	int mahCount = handInMah.size();
	for (int i = 0; i < mahCount - 2; ++i)
	{
		if (handInMah[i] == mah && handInMah[i + 1] == mah && handInMah[i + 2] == mah)
		{
			handInMah.erase(handInMah.begin() + i + 2);
			handInMah.erase(handInMah.begin() + i + 1);
			handInMah.erase(handInMah.begin() + i);
			break;
		}
	}
}

// handInIncludeDrop表示handInMah中是否已经包含了dropMah
std::vector<HU_TYPE> ServerUtility::generateHuType(const std::vector<MAHJONG>& handInMah, const MAHJONG& dropMah, const std::vector<PengGangInfo*>& gangPengList, bool isSelfGet, bool handInIncludeDrop)
{
	// 将数组转换为列表
	std::vector<MAHJONG> handInList;
	std::vector<MAHJONG> pengs;
	std::vector<MAHJONG> gangs;
	int handInCount = handInMah.size();
	for (int i = 0; i < handInCount; ++i)
	{
		handInList.push_back(handInMah[i]);
	}
	// 如果handInMah中不包含dropMah,则需要加到列表中
	if (!handInIncludeDrop)
	{
		int curCount = handInList.size();
		for (int i = 0; i < curCount; ++i)
		{
			if (handInList[i] >= dropMah)
			{
				handInList.insert(handInList.begin() + i, dropMah);
			}
		}
	}
	int gangPengCount = gangPengList.size();
	for (int i = 0; i < gangPengCount; ++i)
	{
		if (gangPengList[i]->mType == AT_GANG)
		{
			pengs.push_back(gangPengList[i]->mMahjong);
		}
		else if (gangPengList[i]->mType == AT_PENG)
		{
			gangs.push_back(gangPengList[i]->mMahjong);
		}
	}
	// 判断胡牌类型
	std::vector<HU_TYPE> huList;
	// 是否为清一色
	if (isQingYiSe(handInList, pengs, gangs))
	{
		huList.push_back(HT_QINGYISE);
	}
	// 如果不满足任意一种番型,则为平胡
	if (huList.size() == 0)
	{
		huList.push_back(HT_NORMAL);
	}
	return huList;
}

bool ServerUtility::isQingYiSe(const std::vector<MAHJONG>& handInMah, const std::vector<MAHJONG>& pengList, const std::vector<MAHJONG>& gangList)
{
	int handInCount = handInMah.size();
	MAHJONG_HUASE curHuase = getHuaSe(handInMah[0]);
	for (int i = 0; i < handInCount; ++i)
	{
		if (getHuaSe(handInMah[i]) != curHuase)
		{
			return false;
		}
	}
	int pengCount = pengList.size();
	for (int i = 0; i < pengCount; ++i)
	{
		if (getHuaSe(pengList[i]) != curHuase)
		{
			return false;
		}
	}
	int gangCount = gangList.size();
	for (int i = 0; i < gangCount; ++i)
	{
		if (getHuaSe(gangList[i]) != curHuase)
		{
			return false;
		}
	}
	return true;
}