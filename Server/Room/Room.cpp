#include "Room.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "CommandHeader.h"

void Room::update(const float& elapsedTime)
{
	// 开始拿牌时,需要由麻将系统给玩家分发牌
	if (mPlayState == MPS_GET_START)
	{
		mCurInterval -= elapsedTime;
		// 从庄家开始发牌
		if (mCurInterval <= 0.0f)
		{
			mCurInterval = ASSIGN_MAHJONG_INTERVAL;
			CharacterPlayer* curPlayer = mPlayerPositionList[mCurAssignPos];
			if (curPlayer == NULL)
			{
				return;
			}

			// 给玩家发牌
			MAHJONG mah = requestGet();
			CommandCharacterGetStartMahjong cmdGetStart(CMD_PARAM);
			cmdGetStart.mMahjong = mah;
			mCommandSystem->pushCommand(&cmdGetStart, curPlayer);
			// 通知其他玩家
			notifyOtherPlayerGetStartMahjong(curPlayer, mah);

			// 判断是否已经拿够了
			bool isDone = false;
			int palyerHandInCount = curPlayer->getCharacterData()->mHandIn.size();
			// 如果是庄家,需要拿够14张牌
			if (mCurAssignPos == mBankerPos)
			{
				isDone = (palyerHandInCount == MAX_HAND_IN_COUNT);
			}
			// 不是庄家则拿13张牌
			else
			{
				isDone = (palyerHandInCount == MAX_HAND_IN_COUNT - 1);
			}
			// 牌拿完时需要重新排列
			if (isDone)
			{
				CommandCharacterReorderMahjong cmdReorder(CMD_PARAM);
				cmdReorder.mPlayerGUID = curPlayer->getGUID();
				mCommandSystem->pushCommand(&cmdReorder, curPlayer);

				// 通知其他玩家有玩家的牌重新排列
				notifyOtherPlayerReorder(curPlayer);

				// 如果是庄家拿完了牌,则进入正常游戏流程
				if (mCurAssignPos == mBankerPos)
				{
					notifyAlPlayerGetStartDone();
					setMahjongState(MPS_NORMAL_GAMING);

					// 通知玩家打出一张牌
					CommandCharacterAskDrop cmdAskDrop(CMD_PARAM);
					mCommandSystem->pushCommand(&cmdAskDrop, curPlayer);
					return;
				}
			}
			mCurAssignPos = (mCurAssignPos + 1) % mMaxPlayer;
		}
	}
}

void Room::joinRoom(CharacterPlayer* player)
{
	CharacterData* data = player->getCharacterData();
	// 第一个加入房间的玩家为庄家
	data->mBanker = (mPlayerList.size() == 0);
	// 加入房间的玩家列表,并且在其中设置玩家的位置
	addPlayer(player);
}

void Room::leaveRoom(CharacterPlayer* player)
{
	CharacterData* data = player->getCharacterData();
	int prePosition = data->mPosition;
	removePlayer(player);
	// 如果是已经准备的玩家离开房间,则需要减少已经准备的玩家计数
	if (data->mReady)
	{
		--mReadyCount;
	}
	// 如果是庄家离开了房间
	if (data->mBanker)
	{
		// 找到下一个玩家设置为庄家
		if (mPlayerList.size() > 0)
		{
			int nextPos = (prePosition + 1) % mMaxPlayer;
			std::map<int, CharacterPlayer*>::iterator iterNext = mPlayerPositionList.find(nextPos);
			while (true)
			{
				if (iterNext == mPlayerPositionList.end())
				{
					iterNext = mPlayerPositionList.begin();
				}
				if (iterNext->second != NULL)
				{
					iterNext->second->getCharacterData()->mBanker = true;
					// 通知房间中的所有玩家有庄家变化
					notifyAllPlayerBanker(iterNext->second->getGUID());
					break;
				}
				else
				{
					++iterNext;
				}
			}
		}
	}
}

void Room::notifyPlayerReady(const CHAR_GUID& playerGUID, const bool& ready)
{
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.find(playerGUID);
	if (iterPlayer == mPlayerList.end())
	{
		return;
	}
	ready ? ++mReadyCount : --mReadyCount;
}

void Room::notifyDiceDone(const CHAR_GUID& playerGUID)
{
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.find(playerGUID);
	if (iterPlayer == mPlayerList.end())
	{
		return;
	}
	++mDiceDoneCount;
}

CharacterPlayer* Room::getMember(const CHAR_GUID& playerID)
{
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.find(playerID);
	if (iterPlayer != mPlayerList.end())
	{
		return iterPlayer->second;
	}
	return NULL;
}

void Room::addPlayer(CharacterPlayer* player)
{
	int playerID = player->getGUID();
	CharacterData* data = player->getCharacterData();
	mPlayerList.insert(std::make_pair(playerID, player));
	data->mPosition = -1;
	// 找到一个空的位置,将玩家设置到该位置上
	std::map<int, CharacterPlayer*>::iterator iterPosition = mPlayerPositionList.begin();
	std::map<int, CharacterPlayer*>::iterator iterPositionEnd = mPlayerPositionList.end();
	for (; iterPosition != iterPositionEnd; ++iterPosition)
	{
		if (iterPosition->second == NULL)
		{
			iterPosition->second = player;
			data->mPosition = iterPosition->first;
			break;
		}
	}
	if (data->mPosition == -1)
	{
		GAME_ERROR("can not find an available position!");
	}
}

void Room::removePlayer(CharacterPlayer* player)
{
	CharacterData* data = player->getCharacterData();
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.find(data->mGUID);
	if (iterPlayer != mPlayerList.end())
	{
		mPlayerList.erase(iterPlayer);
	}
	std::map<int, CharacterPlayer*>::iterator iterPosition = mPlayerPositionList.find(data->mPosition);
	if (iterPosition != mPlayerPositionList.end())
	{
		if (iterPosition->second == player)
		{
			iterPosition->second = NULL;
		}
		else
		{
			GAME_ERROR("player not match position!");
		}
	}
	data->mPosition = -1;
}

void Room::setMahjongState(const MAHJONG_PLAY_STATE& state)
{
	mPlayState = state;
	// 进入等待流程时,就是开始新一局的麻将游戏,需要重置麻将系统的数据
	if (mPlayState == MPS_WAITING)
	{
		reset();
	}
	// 开始掷骰子时,需要计算出掷骰子的结果
	else if (mPlayState == MPS_DICE)
	{
		;
	}
	// 开始拿牌时,需要重置麻将池
	else if (mPlayState == MPS_GET_START)
	{
		resetMahjongPool();
		// 判断当前谁是庄家
		std::map<int, CharacterPlayer*>::iterator iter = mPlayerPositionList.begin();
		std::map<int, CharacterPlayer*>::iterator iterEnd = mPlayerPositionList.end();
		for (; iter != iterEnd; ++iter)
		{
			if (iter->second->getCharacterData()->mBanker)
			{
				mBankerPos = iter->first;
				mCurAssignPos = mBankerPos;
				break;
			}
		}
		if (mBankerPos == -1)
		{
			GAME_ERROR("not find banker!");
		}
	}
	else if (mPlayState == MPS_NORMAL_GAMING)
	{
		mWaitList.clear();
	}
	else if (mPlayState == MPS_WAIT_FOR_ACTION)
	{
		;
	}
	else if (mPlayState == MPS_ENDING)
	{
		;
	}
}

void Room::reset()
{
	mMahjongPool.clear();
	mBankerPos = -1;
	mCurAssignPos = -1;
	mWaitList.clear();
	mReadyCount = 0;
	mDiceDoneCount = 0;
}

void Room::resetMahjongPool()
{
	for (int i = 0; i < (int)M_MAX; ++i)
	{
		for (int j = 0; j < MAX_SINGLE_COUNT; ++j)
		{
			mMahjongPool.push_back((MAHJONG)i);
		}
	}
	// 打乱麻将的顺序
	// 复制一份麻将池,然后从中随机取出放入麻将池中,直到取完
	int mahjongCount = mMahjongPool.size();
	std::vector<MAHJONG> tempPool = mMahjongPool;
	for (int i = 0; i < mahjongCount; ++i)
	{
		int randIndex = txUtility::randomInt(0, tempPool.size() - 1);
		mMahjongPool[i] = tempPool[randIndex];
		// 填充临时麻将池中的空隙
		tempPool[randIndex] = tempPool[mahjongCount - i - 1];
		tempPool.erase(tempPool.begin() + tempPool.size() - 1);
	}
}

// 请求从麻将池中拿一张牌
MAHJONG Room::requestGet()
{
	if (mMahjongPool.size() > 0)
	{
		int index = mMahjongPool.size() - 1;
		MAHJONG mah = mMahjongPool[index];
		mMahjongPool.erase(mMahjongPool.begin() + index);
		return mah;
	}
	return M_MAX;
}

void Room::notifyOtherPlayerGetStartMahjong(CharacterPlayer* exceptPlayer, MAHJONG mah)
{
	std::map<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	std::map<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != exceptPlayer)
		{
			CommandCharacterNotifyOtherPlayerGetStartMahjong cmdNotifyGetStart(CMD_PARAM);
			cmdNotifyGetStart.mPlayerGUID = exceptPlayer->getGUID();
			cmdNotifyGetStart.mMahjong = mah;
			mCommandSystem->pushCommand(&cmdNotifyGetStart, iterPlayer->second);
		}
	}
}

void Room::notifyAlPlayerGetStartDone()
{
	std::map<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	std::map<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyGetStartDone cmdStartDone(CMD_PARAM);
		mCommandSystem->pushCommand(&cmdStartDone, iterPlayer->second);
	}
}

void Room::notifyOtherPlayerReorder(CharacterPlayer* exceptPlayer)
{
	std::map<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	std::map<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != exceptPlayer)
		{
			CommandCharacterReorderMahjong cmdOtherReorder(CMD_PARAM);
			cmdOtherReorder.mPlayerGUID = exceptPlayer->getGUID();
			mCommandSystem->pushCommand(&cmdOtherReorder, iterPlayer->second);
		}
	}
}

void Room::notifyAllPlayerBanker(CHAR_GUID banker)
{
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.begin();
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyBanker cmdNotifyBanker(CMD_PARAM);
		cmdNotifyBanker.mBankerID = banker;
		mCommandSystem->pushCommand(&cmdNotifyBanker, iterPlayer->second);
	}
}

void Room::notifyAllPlayerDiceDone()
{
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.begin();
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyDiceDone cmdDiceDone(CMD_PARAM);
		mCommandSystem->pushCommand(&cmdDiceDone, iterPlayer->second);
	}
}