#ifndef _SC_ASK_ACTION_H_
#define _SC_ASK_ACTION_H_

#include "Packet.h"
#include "GameDefine.h"
#include "MahjongAction.h"
#include "CharacterPlayer.h"

class SCAskAction : public Packet
{
public:
	SCAskAction(const PACKET_TYPE& type)
		:
		Packet(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushArrayParam(mAction, 4);
		pushArrayParam(mActionPlayer, 4);
		pushArrayParam(mDroppedPlayer, 4);
		pushArrayParam(mMahjong, 4);
		pushArrayParam(mHuList, MAX_HU_COUNT);
	}
	void setActionList(const txVector<MahjongAction*>& actionList)
	{
		int count = actionList.size();
		for (int i = 0; i < 4; ++i)
		{
			if (i < count)
			{
				mAction[i] = actionList[i]->mType;
				mActionPlayer[i] = actionList[i]->mActionPlayer->getGUID();
				mDroppedPlayer[i] = actionList[i]->mDroppedPlayer->getGUID();
				mMahjong[i] = actionList[i]->mMah;
				if (actionList[i]->mType == AT_HU)
				{
					setHuList(actionList[i]->mHuList);
				}
			}
			else
			{
				mAction[i] = AT_MAX;
			}
		}
	}
protected:
	void setHuList(const txVector<HU_TYPE>& huList)
	{
		int count = huList.size();
		for (int i = 0; i < MAX_HU_COUNT; ++i)
		{
			if (i < count)
			{
				mHuList[i] = huList[i];
			}
			else
			{
				mHuList[i] = HT_MAX;
			}
		}
	}
public:
	char mAction[4];
	int mActionPlayer[4];
	int mDroppedPlayer[4];
	char mMahjong[4];
	char mHuList[MAX_HU_COUNT];		// 当有胡操作时,该数组中才会有值
};

#endif