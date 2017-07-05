#ifndef _SC_NOTIFY_MAHJONG_END_H_
#define _SC_NOTIFY_MAHJONG_END_H_

#include "Packet.h"
#include "GameDefine.h"

class SCNotifyMahjongEnd : public Packet
{
public:
	SCNotifyMahjongEnd(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushArrayParam(mCharacterGUIDList, MAX_PLAYER);
		pushArrayParam(mMoneyDeltaList, MAX_PLAYER);
	}
	void setList(const std::map<CHAR_GUID, int>& infoList)
	{
		for (int i = 0; i < MAX_PLAYER; ++i)
		{
			mCharacterGUIDList[i] = INVALID_ID;
			mMoneyDeltaList[i] = 0;
		}
		std::map<CHAR_GUID, int>::const_iterator iter = infoList.begin();
		std::map<CHAR_GUID, int>::const_iterator iterEnd = infoList.end();
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