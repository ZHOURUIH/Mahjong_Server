#ifndef _SC_START_GAME_H_
#define _SC_START_GAME_H_

#include "Packet.h"
#include "GameDefine.h"
#include "Utility.h"

class SCStartGame : public Packet
{
public:
	SCStartGame(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushArrayParam(mDice, 2);
		pushParam(mPlayerCount);
		pushArrayParam(mPlayerIDList, MAX_PLAYER);
		pushArrayParam(mHandInList, MAX_PLAYER * MAX_HAND_IN_COUNT);
		pushArrayParam(mHuaList, MAX_PLAYER * MAX_HUA_COUNT);
	}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG("dice : %d, %d", (int)mDice[0], (int)mDice[1]);
	}
	void setDice(char dice0, char dice1)
	{
		mDice[0] = dice0;
		mDice[1] = dice1;
	}
	void setMahjongList(txVector<CHAR_GUID>& playerIDList, txVector<txVector<MAHJONG>>& handIn, txVector<txVector<MAHJONG>>& hua)
	{
		if (handIn.size() != hua.size() || handIn.size() != playerIDList.size())
		{
			LOG_ERROR("mahjong count not match hua count!");
			return;
		}
		// 现将数组全部设置为无效值
		for (int i = 0; i < MAX_PLAYER * MAX_HAND_IN_COUNT; ++i)
		{
			mHandInList[i] = M_MAX;
		}
		for (int i = 0; i < MAX_PLAYER * MAX_HUA_COUNT; ++i)
		{
			mHuaList[i] = M_MAX;
		}
		for (int i = 0; i < MAX_PLAYER; ++i)
		{
			mPlayerIDList[i] = INVALID_ID;
		}
		mPlayerCount = playerIDList.size();
		FOR(playerIDList, int i = 0; i < mPlayerCount; ++i)
		{
			mPlayerIDList[i] = playerIDList[i];
		}
		END(playerIDList);
		FOR(handIn, int i = 0; i < mPlayerCount; ++i)
		{
			auto& playerHandIn = handIn[i];
			int curHandInCount = playerHandIn.size();
			FOR(playerHandIn, int j = 0; j < curHandInCount; ++j)
			{
				if (playerHandIn[j] == M_MAX)
				{
					LOG_ERROR("start hand in mahjong error!");
					break;
				}
				mHandInList[MAX_HAND_IN_COUNT * i + j] = playerHandIn[j];
			}
			END(playerHandIn);
			auto& playerHua = hua[i];
			int curHuaCount = playerHua.size();
			FOR(playerHua, int j = 0; j < curHuaCount; ++j)
			{
				if (!ServerUtility::isHua(playerHua[j]))
				{
					LOG_ERROR("start hua mahjong error!");
					break;
				}
				mHuaList[MAX_HUA_COUNT * i + j] = playerHua[j];
			}
			END(playerHua);
		}
		END(handIn);
	}
public:
	char mDice[2];
	char mPlayerCount;
	CHAR_GUID mPlayerIDList[MAX_PLAYER];
	// 以下数组是将二维数组合成了一维数组
	char mHandInList[MAX_PLAYER * MAX_HAND_IN_COUNT];
	char mHuaList[MAX_PLAYER * MAX_HUA_COUNT];
};

#endif