#include "ServerUtility.h"

txMap<HU_TYPE, int> ServerUtility::mMultipleList;

// handInMah�����Ǵ�С�������������
bool ServerUtility::canHu(txVector<MAHJONG>& handInMah, const MAHJONG& mah)
{
	//����һ���б�
	txVector<MAHJONG> temp = handInMah;
	// Ȼ����˴�����Ƽ�������
	temp.push_back(mah);
	return canHu(temp);
}

bool ServerUtility::canHu(txVector<MAHJONG>& handInMah)
{
	txVector<txVector<MAHJONG>> devide;
	txVector<MahInfo> group;
	toMahjongGroup(handInMah, group);
	return canHu(group, devide);
}

void ServerUtility::toMahjongGroup(txVector<MAHJONG>& list, txVector<MahInfo>& group)
{
	txMap<MAHJONG, MahInfo> groupMap;
	int listSize = list.size();
	FOR_STL(list, int i = 0; i < listSize; ++i)
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
			groupMap.insert(mah, info);
		}
	}
	END_FOR_STL(list);
	txMap<MAHJONG, MahInfo>::iterator iter = groupMap.begin();
	txMap<MAHJONG, MahInfo>::iterator iterEnd = groupMap.end();
	FOR_STL(groupMap, ; iter != iterEnd; ++iter)
	{
		group.push_back(iter->second);
	}
	END_FOR_STL(groupMap);
}

// �õ�ָ���ƵĻ�ɫ
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

bool ServerUtility::isShunzi(txVector<MahInfo>& mahjongList, const int& startIndex)
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
	// ������ͬ��ɫ��
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

void ServerUtility::getHuaseList(txVector<MahInfo>& infoList, txMap<MAHJONG_HUASE, txVector<MAHJONG>> huaseList, bool includeFeng)
{
	int size = infoList.size();
	FOR_STL(infoList, int i = 0; i < size; ++i)
	{
		MAHJONG_HUASE huase = getHuaSe(infoList[i].mMah);
		if (huaseList.find(huase) == huaseList.end())
		{
			if (includeFeng || huase != MH_FENG)
			{
				txVector<MAHJONG> temp;
				temp.push_back(infoList[i].mMah);
				huaseList.insert(huase, temp);
			}
		}
		else
		{
			huaseList[huase].push_back(infoList[i].mMah);
		}
	}
	END_FOR_STL(infoList);
}

bool ServerUtility::canHu(txVector<MahInfo> mahjongList, txVector<txVector<MAHJONG>>& devideList)
{
	// ��������ȱһɫ�Ʋ��ܺ�,
	txMap<MAHJONG_HUASE, txVector<MAHJONG>> huaseList;
	getHuaseList(mahjongList, huaseList);
	if (huaseList.size() > 2)
	{
		return false;
	}
	int allCount = 0;
	int size = mahjongList.size();
	FOR_STL(mahjongList, int i = 0; i < size; ++i)
	{
		allCount += mahjongList[i].mCount;
	}
	END_FOR_STL(mahjongList);
	if (allCount > 2)
	{
		// ȡ�����п��ܵ�˳�Ӻ�������ͬ��,��һ�ж��Ƿ�ɺ�
		int index = 0;
		while (true)
		{
			// û�ҵ�˳��,���˳�ѭ��
			if (index >= (int)mahjongList.size())
			{
				break;
			}
			// �ж��Ƿ���������ͬ��
			if (mahjongList[index].mCount >= 3)
			{
				// �ȱ����б�
				txVector<MahInfo> beforeList = mahjongList;
				// �Ƴ�������ͬ��,�ж�ȥ�����Ƿ�ɺ�
				MAHJONG curMah = mahjongList[index].mMah;
				mahjongList[index].mCount -= 3;
				if (mahjongList[index].mCount == 0)
				{
					mahjongList.erase(mahjongList.begin() + index);
				}
				if (canHu(mahjongList, devideList))
				{
					txVector<MAHJONG> temp;
					temp.push_back(curMah);
					temp.push_back(curMah);
					temp.push_back(curMah);
					devideList.push_back(temp);
					return true;
				}
				// ���ܺ���ԭ��ȥ
				else
				{
					mahjongList = beforeList;
				}
			}
			// �ж��Ƿ���˳��
			if (isShunzi(mahjongList, index))
			{
				// �ȱ����б�
				txVector<MahInfo> beforeList = mahjongList;
				MAHJONG mah0 = mahjongList[index].mMah;
				MAHJONG mah1 = mahjongList[index + 1].mMah;
				MAHJONG mah2 = mahjongList[index + 2].mMah;
				// �Ƴ���˳��,�ж�ȥ�����Ƿ�ɺ�,��Ҫ�Ӻ���ǰ�ж�,�����Ƴ���Ӱ���±�
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
				// ������Ժ�,��ֱ�ӷ���true
				if (canHu(mahjongList, devideList))
				{
					txVector<MAHJONG> temp;
					temp.push_back(mah0);
					temp.push_back(mah1);
					temp.push_back(mah2);
					devideList.push_back(temp);
					return true;
				}
				// ���ܺ�,����Ҫ��˳�ӻ�ԭ���б���,Ȼ�����Ѱ����һ��˳��
				else
				{
					mahjongList = beforeList;
				}
			}
			// �ж�������ͬ��˳�Ӻ󶼲��ܺ�,�ͼ���������
			++index;
		}
		// ���������һ��˳�Ӷ�û���ֿ��Ժ�,���ܺ�
		return false;
	}
	// �ж����ʣ���������Ƿ�Ϊ����
	else
	{
		bool ret = (mahjongList.size() == 1 && mahjongList[0].mCount == 2);
		if (ret)
		{
			txVector<MAHJONG> temp;
			temp.push_back(mahjongList[0].mMah);
			temp.push_back(mahjongList[0].mMah);
			devideList.push_back(temp);
		}
		return ret;
	}
}

bool ServerUtility::canPeng(txVector<MAHJONG>& handInMah, const MAHJONG& mah)
{
	bool ret = false;
	txVector<MahInfo> infoList;
	toMahjongGroup(handInMah, infoList);
	int count = infoList.size();
	FOR_STL(infoList, int i = 0; i < count; ++i)
	{
		if (infoList[i].mCount >= 2 && infoList[i].mMah == mah)
		{
			ret = true;
			break;
		}
	}
	END_FOR_STL(infoList);
	return ret;
}

bool ServerUtility::canGang(txVector<MAHJONG>& handInMah, const MAHJONG& mah)
{
	bool ret = false;
	txVector<MahInfo> infoList;
	toMahjongGroup(handInMah, infoList);
	int count = infoList.size();
	FOR_STL(infoList, int i = 0; i < count; ++i)
	{
		if (infoList[i].mCount == MAX_SINGLE_COUNT - 1 && infoList[i].mMah == mah)
		{
			ret = true;
			break;
		}
	}
	END_FOR_STL(infoList);
	return false;
}

bool ServerUtility::canGang(txVector<MAHJONG>& handInMah)
{
	bool ret = false;
	txVector<MahInfo> infoList;
	toMahjongGroup(handInMah, infoList);
	int count = infoList.size();
	FOR_STL(infoList, int i = 0; i < count; ++i)
	{
		if (infoList[i].mCount == MAX_SINGLE_COUNT)
		{
			ret = true;
			break;
		}
	}
	END_FOR_STL(infoList);
	return ret;
}

void ServerUtility::pengMahjong(txVector<MAHJONG>& handInMah, const MAHJONG& mah)
{
	// ����ǰ����֮ǰ����������
	int mahCount = handInMah.size();
	FOR_STL(handInMah, int i = 0; i < mahCount - 1; ++i)
	{
		if (handInMah[i] == mah && handInMah[i + 1] == mah)
		{
			// �Ӻ���ǰɾ��
			handInMah.erase(handInMah.begin() + i + 1);
			handInMah.erase(handInMah.begin() + i);
			break;
		}
	}
	END_FOR_STL(handInMah);
}

void ServerUtility::gangMahjong(txVector<MAHJONG>& handInMah, const MAHJONG& mah)
{
	// �ܵ�ǰ����֮ǰ�������Ը�
	int mahCount = handInMah.size();
	FOR_STL(handInMah, int i = 0; i < mahCount - 2; ++i)
	{
		if (handInMah[i] == mah && handInMah[i + 1] == mah && handInMah[i + 2] == mah)
		{
			handInMah.erase(handInMah.begin() + i + 2);
			handInMah.erase(handInMah.begin() + i + 1);
			handInMah.erase(handInMah.begin() + i);
			break;
		}
	}
	END_FOR_STL(handInMah);
}

// handInIncludeDrop��ʾhandInMah���Ƿ��Ѿ�������dropMah
txVector<HU_TYPE> ServerUtility::generateHuType(txVector<MAHJONG>& handInMah, const MAHJONG& dropMah, txVector<PengGangInfo*>& gangPengList, bool isSelfGet, bool handInIncludeDrop)
{
	// ������ת��Ϊ�б�
	txVector<MAHJONG> handInList;
	txVector<MAHJONG> pengs;
	txVector<MAHJONG> gangs;
	int handInCount = handInMah.size();
	FOR_STL(handInMah, int i = 0; i < handInCount; ++i)
	{
		handInList.push_back(handInMah[i]);
	}
	END_FOR_STL(handInMah);
	// ���handInMah�в�����dropMah,����Ҫ�ӵ��б���
	if (!handInIncludeDrop)
	{
		int curCount = handInList.size();
		FOR_STL(handInList, int i = 0; i < curCount; ++i)
		{
			if (handInList[i] >= dropMah)
			{
				handInList.insert(handInList.begin() + i, dropMah, false);
				break;
			}
		}
		END_FOR_STL(handInList);
	}
	int gangPengCount = gangPengList.size();
	FOR_STL(gangPengList, int i = 0; i < gangPengCount; ++i)
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
	END_FOR_STL(gangPengList);
	// �жϺ�������
	txVector<HU_TYPE> huList;
	// �Ƿ�Ϊ��һɫ
	if (isQingYiSe(handInList, pengs, gangs))
	{
		huList.push_back(HT_QINGYISE);
	}
	// �������������һ�ַ���,��Ϊƽ��
	if (huList.size() == 0)
	{
		huList.push_back(HT_NORMAL);
	}
	return huList;
}

bool ServerUtility::isQingYiSe(txVector<MAHJONG>& handInMah, txVector<MAHJONG>& pengList, txVector<MAHJONG>& gangList)
{
	bool ret = true;
	int handInCount = handInMah.size();
	MAHJONG_HUASE curHuase = getHuaSe(handInMah[0]);
	// �������Ƿ���������ɫ
	FOR_STL(handInMah, int i = 0; i < handInCount; ++i)
	{
		if (getHuaSe(handInMah[i]) != curHuase)
		{
			ret = false;
			break;
		}
	}
	END_FOR_STL(handInMah);
	if (!ret)
	{
		return ret;
	}
	// �������Ƿ���������ɫ
	int pengCount = pengList.size();
	FOR_STL(pengList, int i = 0; i < pengCount; ++i)
	{
		if (getHuaSe(pengList[i]) != curHuase)
		{
			ret = false;
			break;
		}
	}
	END_FOR_STL(pengList);
	if (!ret)
	{
		return ret;
	}
	// �������Ƿ���������ɫ
	int gangCount = gangList.size();
	FOR_STL(gangList, int i = 0; i < gangCount; ++i)
	{
		if (getHuaSe(gangList[i]) != curHuase)
		{
			ret = false;
			break;
		}
	}
	END_FOR_STL(gangList);
	return ret;
}