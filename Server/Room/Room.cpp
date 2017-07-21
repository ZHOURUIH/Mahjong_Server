#include "Room.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "CommandHeader.h"
#include "ServerUtility.h"
#include "WaitActionInfo.h"
#include "MahjongAction.h"
#include "HuInfo.h"

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
					playerAskDrop(curPlayer);
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
			txMap<int, CharacterPlayer*>::iterator iterNext = mPlayerPositionList.find(nextPos);
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
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.find(playerGUID);
	if (iterPlayer == mPlayerList.end())
	{
		return;
	}
	ready ? ++mReadyCount : --mReadyCount;
}

void Room::notifyDiceDone(const CHAR_GUID& playerGUID)
{
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.find(playerGUID);
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
		txMap<CHAR_GUID, CharacterPlayer*>::iterator iter = mPlayerList.begin();
		txMap<CHAR_GUID, CharacterPlayer*>::iterator iterEnd = mPlayerList.end();
		FOR_STL (mPlayerList, ; iter != iterEnd; ++iter)
		{
			if (iter->second != player)
			{
				txVector<MahjongAction*> checkActionList;
				CharacterData* data = iter->second->getCharacterData();
				// 是否可胡
				if (ServerUtility::canHu(data->mHandIn, mah))
				{
					txVector<HU_TYPE> huList = ServerUtility::generateHuType(data->mHandIn, mah, data->mPengGangList, false, false);
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
		END_FOR_STL(mPlayerList);
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
				endGame(txMap<CharacterPlayer*, HuInfo*>());
			}
		}
	}
}

void Room::notifyPlayerGet(CharacterPlayer* player, const MAHJONG& mah)
{
	// 判断是否可胡或者可杠
	CharacterData* data = player->getCharacterData();
	txVector<MahjongAction*> actionList;
	// 是否可胡
	if (ServerUtility::canHu(data->mHandIn))
	{
		txVector<HU_TYPE> huList = ServerUtility::generateHuType(data->mHandIn, mah, data->mPengGangList, true, true);
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
		FOR_STL (data->mPengGangList, int i = 0; i < pengGangCount; ++i)
		{
			if (data->mPengGangList[i]->mMahjong == mah && data->mPengGangList[i]->mType == AT_PENG)
			{
				MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_GANG, player, player, mah);
				actionList.push_back(action);
				break;
			}
		}
		END_FOR_STL(data->mPengGangList)
	}
	if (actionList.size() > 0)
	{
		// 如果有可以操作的行为,则还需要添加Pass行为
		MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_PASS, player, player, M_MAX);
		actionList.push_back(action);
		askPlayerAction(player, player, mah, actionList);
	}
	else
	{
		// 没有任何操作则通知玩家需要打一张牌出来
		playerAskDrop(player);
	}
}

void Room::playerConfirmAction(CharacterPlayer* player, const ACTION_TYPE& type)
{
	txMap<CharacterPlayer*, WaitActionInfo*>::iterator iterWait = mWaitList.find(player);
	if (iterWait == mWaitList.end())
	{
		GAME_ERROR("player has no action : name : %s, action : %d", player->getName().c_str(), type);
		return;
	}
	MahjongAction* action = NULL;
	txVector<MahjongAction*>& actionList = iterWait->second->mActionList;
	int actionCount = actionList.size();
	FOR_STL (actionList, int i = 0; i < actionCount; ++i)
	{
		if (actionList[i]->mType == type)
		{
			action = actionList[i];
			break;
		}
	}
	END_FOR_STL(actionList);
	if (action == NULL)
	{
		return;
	}
	iterWait->second->mConfirmedAction = action;
	// 胡牌的优先级最高,如果有玩家选择胡牌,则忽略其他玩家的操作
	if (action->mType == AT_HU)
	{
		// 查找所有可以胡牌的玩家
		txMap<CharacterPlayer*, HuInfo*> huPlayerList;
		txMap<CharacterPlayer*, WaitActionInfo*>::iterator iter = mWaitList.begin();
		txMap<CharacterPlayer*, WaitActionInfo*>::iterator iterEnd = mWaitList.end();
		FOR_STL(mWaitList, ; iter != iterEnd; ++iter)
		{
			WaitActionInfo* waitInfo = iter->second;
			int count = waitInfo->mActionList.size();
			FOR_STL(waitInfo->mActionList, int i = 0; i < count; ++i)
			{
				MahjongAction* mahjongAction = waitInfo->mActionList[i];
				if (mahjongAction->mType == AT_HU)
				{
					HuInfo* huInfo = TRACE_NEW(HuInfo, huInfo);
					huInfo->mHuPlayer = iter->first;
					huInfo->mDroppedPlayer = waitInfo->mDroppedPlayer;
					huInfo->mMahjong = waitInfo->mMahjong;
					huInfo->mHuList = mahjongAction->mHuList;
					huPlayerList.insert(iter->first, huInfo);
					break;
				}
			}
			END_FOR_STL(waitInfo->mActionList);
		}
		END_FOR_STL(mWaitList);
		playerHu(huPlayerList);
		// 玩家胡牌后则结束游戏
		endGame(huPlayerList);
	}
	else
	{
		bool allConfirm = true;
		CharacterPlayer* highestActionPlayer = NULL;
		MahjongAction* highestAction = NULL;
		txMap<CharacterPlayer*, WaitActionInfo*>::iterator iter = mWaitList.begin();
		txMap<CharacterPlayer*, WaitActionInfo*>::iterator iterEnd = mWaitList.end();
		FOR_STL(mWaitList, ; iter != iterEnd; ++iter)
		{
			WaitActionInfo* info = iter->second;
			if (info->mConfirmedAction == NULL)
			{
				allConfirm = false;
				break;
			}
			if (highestAction == NULL || highestAction->mType > info->mConfirmedAction->mType)
			{
				highestAction = info->mConfirmedAction;
				highestActionPlayer = info->mPlayer;
			}
		}
		END_FOR_STL(mWaitList);
		// 如果全部玩家都已经确认操作了,允许优先级最高的操作进行
		if (allConfirm)
		{
			// 先获得信息,因为在设置状态时会将列表清空
			WaitActionInfo* info = mWaitList[highestActionPlayer];
			// 游戏状态设置为正常游戏
			setMahjongState(MPS_NORMAL_GAMING);
			if (highestAction->mType == AT_GANG)
			{
				playerGang(info->mPlayer, info->mDroppedPlayer, info->mMahjong);
				// 杠完牌需要重新排列
				playerReorderMahjong(info->mPlayer);
				// 还有牌,玩家杠了一张牌以后需要再摸一张
				if (mMahjongPool.size() > 0)
				{
					MAHJONG mah = requestGet();
					playerGetMahjong(mah, info->mPlayer);
					notifyPlayerGet(info->mPlayer, mah);
				}
				// 没有牌了则平局
				else
				{
					endGame(txMap<CharacterPlayer*, HuInfo*>());
				}
			}
			else if (highestAction->mType == AT_PENG)
			{
				playerPeng(info->mPlayer, info->mDroppedPlayer, info->mMahjong);
				playerAskDrop(info->mPlayer);
			}
			else if (highestAction->mType == AT_PASS)
			{
				// 如果是自己摸了一张牌,选择了pass,则需要自己打一张牌出来
				if (info->mDroppedPlayer == info->mPlayer)
				{
					playerAskDrop(info->mPlayer);
				}
				else
				{
					// 还有牌则通知下一家摸牌
					if (mMahjongPool.size() > 0)
					{
						int nextPosition = (info->mDroppedPlayer->getCharacterData()->mPosition + 1) % mMaxPlayer;
						CharacterPlayer* nextPlayer = getMemberByPosition(nextPosition);
						MAHJONG mah = requestGet();
						playerGetMahjong(mah, nextPlayer);
						notifyPlayerGet(nextPlayer, mah);
					}
					// 没有牌了则平局
					else
					{
						endGame(txMap<CharacterPlayer*, HuInfo*>());
					}
				}
			}
		}
	}
}

void Room::endGame(txMap<CharacterPlayer*, HuInfo*>& huPlayerList)
{
	// 设置为结束状态
	setMahjongState(MPS_ENDING);
	// 通知所有玩家本局结束,计算所有玩家的输赢,暂时不计算杠的牌
	txMap<CharacterPlayer*, int> moneyDeltaList;
	txMap<int, CharacterPlayer*>::iterator iter = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, int i = 0; iter != iterEnd; (++iter, ++i))
	{
		moneyDeltaList.insert(iter->second, 0);
	}
	END_FOR_STL(mPlayerPositionList);
	txMap<CharacterPlayer*, HuInfo*>::iterator iterHu = huPlayerList.begin();
	txMap<CharacterPlayer*, HuInfo*>::iterator iterHuEnd = huPlayerList.end();
	FOR_STL(huPlayerList, ; iterHu != iterHuEnd; ++iterHu)
	{
		int huCount = iterHu->second->mHuList.size();
		FOR_STL(iterHu->second->mHuList, int i = 0; i < huCount; ++i)
		{
			huCount += ServerUtility::getHuMultiple(iterHu->second->mHuList[i]);
		}
		END_FOR_STL(iterHu->second->mHuList);
		int money = 100 * huCount;
		// 自摸,所有人都一样
		if (iterHu->second->mDroppedPlayer == iterHu->second->mHuPlayer)
		{
			txMap<CharacterPlayer*, int>::iterator iterMoney = moneyDeltaList.begin();
			txMap<CharacterPlayer*, int>::iterator iterMoneyEnd = moneyDeltaList.end();
			FOR_STL(moneyDeltaList, ; iterMoney != iterMoneyEnd; ++iterMoney)
			{
				if (iterMoney->first != iterHu->first)
				{
					iterMoney->second -= money;
				}
				else
				{
					iterMoney->second += (mPlayerPositionList.size() - 1)* money;
				}
			}
			END_FOR_STL(moneyDeltaList);
		}
		// 别人点的炮
		else
		{
			moneyDeltaList[iterHu->first] += money;
			moneyDeltaList[iterHu->second->mDroppedPlayer] -= money;
		}
	}
	END_FOR_STL(huPlayerList);
	notifyAllPlayerMahjongEnd(moneyDeltaList);
}

// 询问玩家要选择哪种操作
void Room::askPlayerAction(CharacterPlayer* player, CharacterPlayer* droppedPlayer, const MAHJONG& mah, const txVector<MahjongAction*>& actionList)
{
	if (actionList.size() == 0)
	{
		GAME_ERROR("has no action");
		return;
	}
	// 将行为放入列表
	WaitActionInfo* info = TRACE_NEW(WaitActionInfo, info, player, droppedPlayer, actionList, mah);
	mWaitList.insert(player, info);
	// 设置状态为等待玩家确认操作
	setMahjongState(MPS_WAIT_FOR_ACTION);
	// 询问玩家进行什么操作
	playerAskAction(player, actionList);
}

CharacterPlayer* Room::getMember(const CHAR_GUID& playerID)
{
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.find(playerID);
	if (iterPlayer != mPlayerList.end())
	{
		return iterPlayer->second;
	}
	return NULL;
}

CharacterPlayer* Room::getMemberByPosition(const CHAR_GUID& playerID)
{
	txMap<int, CharacterPlayer*>::iterator iter = mPlayerPositionList.find(playerID);
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
	mPlayerList.insert(playerID, player);
	data->mPosition = -1;
	// 找到一个空的位置,将玩家设置到该位置上
	txMap<int, CharacterPlayer*>::iterator iterPosition = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPositionEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPosition != iterPositionEnd; ++iterPosition)
	{
		if (iterPosition->second == NULL)
		{
			iterPosition->second = player;
			data->mPosition = iterPosition->first;
			break;
		}
	}
	END_FOR_STL(mPlayerPositionList);
	if (data->mPosition == -1)
	{
		GAME_ERROR("can not find an available position!");
	}
}

void Room::removePlayer(CharacterPlayer* player)
{
	CharacterData* data = player->getCharacterData();
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.find(data->mGUID);
	if (iterPlayer != mPlayerList.end())
	{
		mPlayerList.erase(iterPlayer);
	}
	txMap<int, CharacterPlayer*>::iterator iterPosition = mPlayerPositionList.find(data->mPosition);
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
		txMap<int, CharacterPlayer*>::iterator iter = mPlayerPositionList.begin();
		txMap<int, CharacterPlayer*>::iterator iterEnd = mPlayerPositionList.end();
		FOR_STL(mPlayerPositionList, ; iter != iterEnd; ++iter)
		{
			if (iter->second->getCharacterData()->mBanker)
			{
				mBankerPos = iter->first;
				mCurAssignPos = mBankerPos;
				break;
			}
		}
		END_FOR_STL(mPlayerPositionList);
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
		txMap<CharacterPlayer*, WaitActionInfo*>::iterator iter = mWaitList.begin();
		txMap<CharacterPlayer*, WaitActionInfo*>::iterator iterEnd = mWaitList.end();
		FOR_STL(mWaitList, ; iter != iterEnd; ++iter)
		{
			int actionCount = iter->second->mActionList.size();
			FOR_STL(iter->second->mActionList, int i = 0; i < actionCount; ++i)
			{
				TRACE_DELETE(iter->second->mActionList[i]);
			}
			END_FOR_STL(iter->second->mActionList);
			TRACE_DELETE(iter->second);
		}
		END_FOR_STL(mWaitList);
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
	txMap<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
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
	END_FOR_STL(mPlayerPositionList);
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
	txVector<MAHJONG> tempPool = mMahjongPool;
	FOR_STL(mMahjongPool, int i = 0; i < mahjongCount; ++i)
	{
		int randIndex = txUtility::randomInt(0, tempPool.size() - 1);
		mMahjongPool[i] = tempPool[randIndex];
		// 填充临时麻将池中的空隙
		tempPool[randIndex] = tempPool[mahjongCount - i - 1];
		tempPool.erase(tempPool.begin() + tempPool.size() - 1);
	}
	END_FOR_STL(mMahjongPool);
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
	txMap<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyGetStartDone cmdStartDone(CMD_PARAM);
		mCommandSystem->pushCommand(&cmdStartDone, iterPlayer->second);
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::notifyAllPlayerBanker(const CHAR_GUID& banker)
{
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.begin();
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerList.end();
	FOR_STL(mPlayerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyBanker cmdNotifyBanker(CMD_PARAM);
		cmdNotifyBanker.mBankerID = banker;
		mCommandSystem->pushCommand(&cmdNotifyBanker, iterPlayer->second);
	}
	END_FOR_STL(mPlayerList);
}

void Room::notifyAllPlayerDiceDone()
{
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.begin();
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerList.end();
	FOR_STL(mPlayerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyDiceDone cmdDiceDone(CMD_PARAM);
		mCommandSystem->pushCommand(&cmdDiceDone, iterPlayer->second);
	}
	END_FOR_STL(mPlayerList);
}

void Room::notifyAllPlayerMahjongEnd(txMap<CharacterPlayer*, int>& moneyDeltaList)
{
	txMap<int, CharacterPlayer*>::iterator iterPos = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPosEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPos != iterPosEnd; ++iterPos)
	{
		CommandCharacterMahjongEnd cmd(CMD_PARAM);
		cmd.mMoneyDeltaList = moneyDeltaList;
		mCommandSystem->pushCommand(&cmd, iterPos->second);
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerGetStartMahjong(const MAHJONG& mah, CharacterPlayer* player)
{
	CommandCharacterGetStartMahjong cmdGetStart(CMD_PARAM);
	cmdGetStart.mMahjong = mah;
	mCommandSystem->pushCommand(&cmdGetStart, player);
	// 通知其他玩家
	txMap<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerGetStartMahjong cmdNotifyGetStart(CMD_PARAM);
			cmdNotifyGetStart.mPlayerGUID = player->getGUID();
			cmdNotifyGetStart.mMahjong = mah;
			mCommandSystem->pushCommand(&cmdNotifyGetStart, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerGetMahjong(const MAHJONG& mah, CharacterPlayer* player)
{
	CommandCharacterGetMahjong cmdGetStart(CMD_PARAM);
	cmdGetStart.mMahjong = mah;
	mCommandSystem->pushCommand(&cmdGetStart, player);
	// 通知其他玩家
	txMap<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerGetMahjong cmdNotifyGetStart(CMD_PARAM);
			cmdNotifyGetStart.mPlayerGUID = player->getGUID();
			cmdNotifyGetStart.mMahjong = mah;
			mCommandSystem->pushCommand(&cmdNotifyGetStart, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerReorderMahjong(CharacterPlayer* player)
{
	CommandCharacterReorderMahjong cmdReorder(CMD_PARAM);
	mCommandSystem->pushCommand(&cmdReorder, player);

	// 通知其他玩家有玩家的牌重新排列
	txMap<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerReorderMahjong cmdOtherReorder(CMD_PARAM);
			cmdOtherReorder.mPlayerGUID = player->getGUID();
			mCommandSystem->pushCommand(&cmdOtherReorder, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerHu(txMap<CharacterPlayer*, HuInfo*>& huInfoList)
{
	txVector<CharacterPlayer*> huPlayerList;
	txVector<txVector<HU_TYPE>> huList;
	txMap<CharacterPlayer*, HuInfo*>::iterator iterInfo = huInfoList.begin();
	txMap<CharacterPlayer*, HuInfo*>::iterator iterInfoEnd = huInfoList.end();
	FOR_STL(huInfoList, ; iterInfo != iterInfoEnd; ++iterInfo)
	{
		huPlayerList.push_back(iterInfo->first);
		huList.push_back(iterInfo->second->mHuList);
	}
	END_FOR_STL(huInfoList);
	// 通知所有玩家
	txMap<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterPlayerHu cmdOtherHu(CMD_PARAM);
		cmdOtherHu.mHuPlayerList = huPlayerList;
		cmdOtherHu.mHuList = huList;
		mCommandSystem->pushCommand(&cmdOtherHu, iterPlayer->second);
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerGang(CharacterPlayer* player, CharacterPlayer* droppedPlayer, const MAHJONG& mah)
{
	CommandCharacterGang cmdGang(CMD_PARAM);
	cmdGang.mMahjong = mah;
	cmdGang.mDroppedPlayer = droppedPlayer;
	mCommandSystem->pushCommand(&cmdGang, player);
	// 通知其他玩家
	txMap<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerGang cmdOtherGang(CMD_PARAM);
			cmdOtherGang.mOtherPlayer = player;
			cmdOtherGang.mDroppedPlayer = droppedPlayer;
			cmdOtherGang.mMahjong = mah;
			mCommandSystem->pushCommand(&cmdOtherGang, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerPeng(CharacterPlayer* player, CharacterPlayer* droppedPlayer, const MAHJONG& mah)
{
	CommandCharacterPeng cmdPeng(CMD_PARAM);
	cmdPeng.mMahjong = mah;
	cmdPeng.mDroppedPlayer = droppedPlayer;
	mCommandSystem->pushCommand(&cmdPeng, player);
	// 通知其他玩家
	txMap<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerPeng cmdOtherPeng(CMD_PARAM);
			cmdOtherPeng.mOtherPlayer = player;
			cmdOtherPeng.mDroppedPlayer = droppedPlayer;
			cmdOtherPeng.mMahjong = mah;
			mCommandSystem->pushCommand(&cmdOtherPeng, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerPass(CharacterPlayer* player, CharacterPlayer* droppedPlayer, const MAHJONG& mah)
{
	CommandCharacterPass cmdPass(CMD_PARAM);
	cmdPass.mDroppedPlayer = droppedPlayer;
	cmdPass.mMahjong = mah;
	mCommandSystem->pushCommand(&cmdPass, player);
	// 通知其他玩家
	txMap<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerPass cmdOtherPass(CMD_PARAM);
			cmdOtherPass.mOtherPlayer = player;
			cmdOtherPass.mDroppedPlayer = droppedPlayer;
			cmdOtherPass.mMahjong = mah;
			mCommandSystem->pushCommand(&cmdOtherPass, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerAskDrop(CharacterPlayer* player)
{
	CommandCharacterAskDrop cmdAskDrop(CMD_PARAM);
	mCommandSystem->pushCommand(&cmdAskDrop, player);
	// 通知其他玩家
	txMap<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerAskDrop cmdOtherAskDrop(CMD_PARAM);
			cmdOtherAskDrop.mOtherPlayer = player;
			mCommandSystem->pushCommand(&cmdOtherAskDrop, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerAskAction(CharacterPlayer* player, const txVector<MahjongAction*>& actionList)
{
	CommandCharacterAskAction cmdAskAction(CMD_PARAM);
	cmdAskAction.mActionList = actionList;
	mCommandSystem->pushCommand(&cmdAskAction, player);
	// 通知其他玩家
	txMap<int, CharacterPlayer*>::iterator iterPlayer = mPlayerPositionList.begin();
	txMap<int, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerAskAction cmdOtherAskAction(CMD_PARAM);
			cmdOtherAskAction.mOtherPlayer = player;
			mCommandSystem->pushCommand(&cmdOtherAskAction, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}