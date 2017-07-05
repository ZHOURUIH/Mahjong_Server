#ifndef _SC_PLAYER_HU_H_
#define _SC_PLAYER_HU_H_

#include "Packet.h"
#include "GameDefine.h"
#include "CharacterPlayer.h"

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
		pushArrayParam(mHuPlayerGUID, MAX_PLAYER - 1);
		pushArrayParam(mHuList, MAX_HU_COUNT * (MAX_PLAYER - 1));
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG(PACKET_EMPTY);
	}
	void setHuPlayer(const std::vector<CharacterPlayer*>& huPlayerList)
	{
		for (int i = 0; i < MAX_PLAYER; ++i)
		{
			if (i < (int)huPlayerList.size())
			{
				mHuPlayerGUID[i] = huPlayerList[i]->getGUID();
			}
			else
			{
				mHuPlayerGUID[i] = INVALID_ID;
			}
		}
	}
	void setHuList(const std::vector<std::vector<HU_TYPE>>& huList)
	{
		int playerCount = huList.size();
		for (int i = 0; i < MAX_PLAYER - 1; ++i)
		{
			int huCount = i < playerCount ? huList[i].size() : 0;
			for (int j = 0; j < MAX_HU_COUNT; ++j)
			{
				if (mType == AT_HU && j < huCount && i < huCount)
				{
					mHuList[i * MAX_HU_COUNT + j] = huList[i][j];
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