#ifndef _SC_PLAYER_HU_H_
#define _SC_PLAYER_HU_H_

#include "Packet.h"
#include "GameDefine.h"

class SCPlayerHu : public Packet
{
public:
	SCPlayerHu(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mDroppedPlayerGUID);
		pushParam(mMahjong);
		pushArrayParam(mHuList, MAX_HU_COUNT);
	}
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG(PACKET_EMPTY);
	}
	void setHuList(const std::vector<HU_TYPE>& huList)
	{
		int count = huList.size();
		for (int i = 0; i < MAX_HU_COUNT; ++i)
		{
			if (mType == AT_HU && i < count)
			{
				mHuList[i] = huList[i];
			}
			else
			{
				mHuList[i] = HT_NONE;
			}
		}
	}
public:
	int mDroppedPlayerGUID;
	char mMahjong;
	char mHuList[MAX_HU_COUNT];
};

#endif