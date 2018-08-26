#ifndef _SC_PLAYER_HU_H_
#define _SC_PLAYER_HU_H_

#include "Packet.h"
#include "GameDefine.h"
#include "CharacterPlayer.h"

class SCPlayerHu : public Packet
{
public:
	SCPlayerHu(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushArrayParam(mHuPlayerGUID, MAX_PLAYER - 1);
		pushArrayParam(mHuList, MAX_HU_COUNT * (MAX_PLAYER - 1));
	}
	void setHuList(const txVector<std::pair<CharacterPlayer*, txVector<HU_TYPE>>>& huList)
	{
		int playerCount = huList.size();
		for (int i = 0; i < MAX_PLAYER - 1; ++i)
		{
			if (i < playerCount)
			{
				mHuPlayerGUID[i] = huList[i].first->getGUID();
			}
			else
			{
				mHuPlayerGUID[i] = INVALID_INT_ID;
			}
		}

		for (int i = 0; i < MAX_PLAYER - 1; ++i)
		{
			int huCount = i < playerCount ? playerCount : 0;
			for (int j = 0; j < MAX_HU_COUNT; ++j)
			{
				if (j < huCount && i < playerCount)
				{
					mHuList[i * MAX_HU_COUNT + j] = huList[i].second[j];
				}
				else
				{
					mHuList[i * MAX_HU_COUNT + j] = HT_MAX;
				}
			}
		}
	}
public:
	int mHuPlayerGUID[MAX_PLAYER - 1];
	char mHuList[MAX_HU_COUNT * (MAX_PLAYER - 1)];
};

#endif