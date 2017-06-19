#include "Room.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "CommandHeader.h"
#include "ServerUtility.h"
#include "WaitActionInfo.h"
#include "MahjongAction.h"

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
			playerGetStartMahjong(requestGet(), curPlayer);

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
				playerReorderMahjong(curPlayer);

				// 如果是庄家拿完了牌,则进入正常游戏流程
				if (mCurAssignPos == mBankerPos)
				{
					notifyAllPlayerGetStartDone();
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

void Room::notifyPlayerDrop(CharacterPlayer* player, const MAHJONG& mah)
{
	// 正常游戏过程中,玩家打了一张牌后,判断其他玩家是否有碰或者杠,该下家摸牌
	if (mPlayState == MPS_NORMAL_GAMING)
	{
		// 判断其他玩家是否可以碰或者杠
		bool hasAction = false;
		std::map<CHAR_GUID, CharacterPlayer*>::iterator iter = mPlayerList.begin();
		std::map<CHAR_GUID, CharacterPlayer*>::iterator iterEnd = mPlayerList.end();
		for (; iter != iterEnd; ++iter)
		{
			if (iter->second != player)
			{
				std::vector<MahjongAction*> checkActionList;
				CharacterData* data = iter->second->getCharacterData();
				// 是否可胡
				if (ServerUtility::canHu(data->mHandIn, mah))
				{
					std::vector<HU_TYPE> huList = ServerUtility::generateHuType(data->mHandIn, mah, data->mPengGangList, false, false);
					MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_HU, iter->second, player, mah, huList);
					checkActionList.push_back(action);
				}
				// 是否可杠
				if (ServerUtility::canGang(data->mHandIn, mah))
				{
					MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_GANG, iter->second, player, mah);
					checkActionList.push_back(action);
				}
				// 是否可碰
				if (ServerUtility::canPeng(data->mHandIn, mah))
				{
					MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_PENG, iter->second, player, mah);
					checkActionList.push_back(action);
				}
				if (checkActionList.size() > 0)
				{
					hasAction = true;
					// 添加pass操作
					MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_PASS, iter->second, player, mah);
					checkActionList.push_back(action);
					askPlayerAction(iter->second, player, mah, checkActionList);
				}
			}
		}
		// 没有人需要这张牌
		if (!hasAction)
		{
			// 该下家摸牌
			if (mMahjongPool.size() > 0)
			{
				int nextPosition = (player->getCharacterData()->mPosition + 1) % MAX_PLAYER;
				CharacterPlayer* nextPlayer = getMemberByPosition(nextPosition);
				MAHJONG mahjong = requestGet();
				playerGetMahjong(mahjong, nextPlayer);
				// 判断摸牌后的操作
				notifyPlayerGet(nextPlayer, mahjong);
			}
			// 牌已经摸完了,则本局为平局
			else
			{
				endGame(NULL, M_MAX, std::vector<HU_TYPE>());
			}
		}
	}
}

void Room::notifyPlayerGet(CharacterPlayer* player, const MAHJONG& mah)
{
	// 判断是否可胡或者可杠
	CharacterData* data = player->getCharacterData();
	std::vector<MahjongAction*> actionList;
	// 是否可胡
	if (ServerUtility::canHu(data->mHandIn))
	{
		std::vector<HU_TYPE> huList = ServerUtility::generateHuType(data->mHandIn, mah, data->mPengGangList, true, true);
		MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_HU, player, player, mah, huList);
		actionList.push_back(action);
	}
	// 是否可以杠
	else if (ServerUtility::canGang(data->mHandIn))
	{
		MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_GANG, player, player, mah);
		actionList.push_back(action);
	}
	// 摸了一张自己碰的牌,可以开杠
	else
	{
		int pengGangCount = data->mPengGangList.size();
		for (int i = 0; i < pengGangCount; ++i)
		{
			if (data->mPengGangList[i]->mMahjong == mah
				&& data->mPengGangList[i]->mType == AT_PENG)
			{
				MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_GANG, player, player, mah);
				actionList.push_back(action);
				break;
			}
		}
	}
	if (actionList.size() > 0)
	{
		// 如果有可以操作的行为,则还需要添加Pass行为
		MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_PASS, player, NULL, M_MAX);
		actionList.push_back(action);
		askPlayerAction(player, player, mah, actionList);
	}
	else
	{
		// 没有任何操作则通知玩家需要打一张牌出来
		CommandCharacterAskDrop cmdAskDrop(CMD_PARAM);
		mCommandSystem->pushCommand(&cmdAskDrop, player);
	}
}

void Room::endGame(CharacterPlayer* player, const MAHJONG& mahjong, const std::vector<HU_TYPE>& huList)
{
	;
}

// 询问玩家要选择哪种操作
void Room::askPlayerAction(CharacterPlayer* player, CharacterPlayer* droppedPlayer, const MAHJONG& mah, const std::vector<MahjongAction*>& actionList)
{
	if (actionList.size() == 0)
	{
		GAME_ERROR("has no action");
		return;
	}
	// 将行为放入列表
	WaitActionInfo* info = TRACE_NEW(WaitActionInfo, info, player, droppedPlayer, actionList, mah);
	mWaitList.insert(std::make_pair(player, info));
	// 设置状态为等待玩家确认操作
	setMahjongState(MPS_WAIT_FOR_ACTION);
	// 询问玩家进行什么操作
	CommandCharacterAskAction cmd(CMD_PARAM);
	cmd.mActionList = actionList;
	mCommandSystem->pushCommand(&cmd, player);
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

CharacterPlayer* Room::getMemberByPosition(const CHAR_GUID& playerID)
{
	std::map<int, CharacterPlayer*>::iterator iter = mPlayerPositionList.find(playerID);
	if (iter != mPlayerPositionList.end())
	{
		return iter->second;
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

void Room::requestDrop(CharacterPlayer* player, const int& index)
{
	MAHJONG mahjong = player->getCharacterData()->mHandIn[index];
	CommandCharacterDrop cmdDrop(CMD_PARAM);
	cmdDrop.mIndex = index;
	cmdDrop.mMahjong = mahjong;
	mCommandSystem->pushCommand(&cmdDrop, player);

	// 通知其他玩家
	std::map<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	std::map<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerDrop cmd(CMD_PARAM);
			cmd.mIndex = index;
			cmd.mPlayerGUID = player->getGUID();
			cmd.mMahjong = cmdDrop.mMahjong;
			mCommandSystem->pushCommand(&cmd, iterPlayer->second);
		}
	}
	notifyPlayerDrop(player, mahjong);
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

void Room::notifyAllPlayerGetStartDone()
{
	std::map<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	std::map<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyGetStartDone cmdStartDone(CMD_PARAM);
		mCommandSystem->pushCommand(&cmdStartDone, iterPlayer->second);
	}
}

void Room::notifyAllPlayerBanker(const CHAR_GUID& banker)
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

void Room::playerGetStartMahjong(const MAHJONG& mah, CharacterPlayer* player)
{
	CommandCharacterGetStartMahjong cmdGetStart(CMD_PARAM);
	cmdGetStart.mMahjong = mah;
	mCommandSystem->pushCommand(&cmdGetStart, player);
	// 通知其他玩家
	std::map<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	std::map<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerGetStartMahjong cmdNotifyGetStart(CMD_PARAM);
			cmdNotifyGetStart.mPlayerGUID = player->getGUID();
			cmdNotifyGetStart.mMahjong = mah;
			mCommandSystem->pushCommand(&cmdNotifyGetStart, iterPlayer->second);
		}
	}
}

void Room::playerGetMahjong(const MAHJONG& mah, CharacterPlayer* player)
{
	CommandCharacterGetMahjong cmdGetStart(CMD_PARAM);
	cmdGetStart.mMahjong = mah;
	mCommandSystem->pushCommand(&cmdGetStart, player);
	// 通知其他玩家
	std::map<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	std::map<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerGetMahjong cmdNotifyGetStart(CMD_PARAM);
			cmdNotifyGetStart.mPlayerGUID = player->getGUID();
			cmdNotifyGetStart.mMahjong = mah;
			mCommandSystem->pushCommand(&cmdNotifyGetStart, iterPlayer->second);
		}
	}
}

void Room::playerReorderMahjong(CharacterPlayer* player)
{
	CommandCharacterReorderMahjong cmdReorder(CMD_PARAM);
	mCommandSystem->pushCommand(&cmdReorder, player);

	// 通知其他玩家有玩家的牌重新排列
	std::map<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	std::map<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerReorderMahjong cmdOtherReorder(CMD_PARAM);
			cmdOtherReorder.mPlayerGUID = player->getGUID();
			mCommandSystem->pushCommand(&cmdOtherReorder, iterPlayer->second);
		}
	}
}