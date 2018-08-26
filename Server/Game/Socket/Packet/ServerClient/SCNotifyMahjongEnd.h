﻿#ifndef _SC_NOTIFY_MAHJONG_END_H_
#define _SC_NOTIFY_MAHJONG_END_H_

#include "Packet.h"
#include "GameDefine.h"

class SCNotifyMahjongEnd : public Packet
{
public:
	SCNotifyMahjongEnd(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushArrayParam(mCharacterGUIDList, MAX_PLAYER);
		pushArrayParam(mMoneyDeltaList, MAX_PLAYER);
	}
	void setList(txMap<CHAR_GUID, int>& infoList)
	{
		for (int i = 0; i < MAX_PLAYER; ++i)
		{
			mCharacterGUIDList[i] = INVALID_INT_ID;
			mMoneyDeltaList[i] = 0;
		}
		auto iter = infoList.begin();
		auto iterEnd = infoList.end();
		for (int i = 0; iter != iterEnd; ++iter, ++i)
		{
			mCharacterGUIDList[i] = iter->first;
			mMoneyDeltaList[i] = iter->second;
		}
	}
public:
	int mCharacterGUIDList[MAX_PLAYER];
	int mMoneyDeltaList[MAX_PLAYER];
};

#endif