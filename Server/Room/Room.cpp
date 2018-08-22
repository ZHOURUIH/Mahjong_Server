#include "Room.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "CommandHeader.h"
#include "ServerUtility.h"
#include "WaitActionInfo.h"
#include "MahjongAction.h"
#include "HuInfo.h"
#include "Utility.h"

Room::Room(const int& id)
	:
	txCommandReceiver("room" + StringUtility::intToString(id)),
	mID(id),
	mMaxPlayer(MAX_PLAYER),
	mLockRoom(false),
	mDiceDoneCount(0),
	mPlayState(MPS_WAITING),
	mBankerPos(-1),
	mCurAssignPos(-1),
	mCurInterval(0.0f)
{
	for (int i = 0; i < mMaxPlayer; ++i)
	{
		mPlayerPositionList.insert(i, (CharacterPlayer*)NULL);
	}
}

void Room::update(float elapsedTime)
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

			// 给玩家发牌,如果摸到花牌,则需要将花牌拿出来,直到摸到一张不是花牌的
			while (true)
			{
				MAHJONG curMahjong = requestGet();
				playerGetStartMahjong(curMahjong, curPlayer);

				// 检测是否是花牌,如果是花牌,则需要抽出来,重新摸一张牌
				if (ServerUtility::isHua(curMahjong))
				{
					playerShowHua(curPlayer, curPlayer->getCharacterData()->mHandIn.size() - 1, curMahjong);
				}
				else
				{
					break;
				}
			}

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
	// 如果是庄家离开了房间
	if (data->mBanker)
	{
		// 找到下一个玩家设置为庄家
		if (mPlayerList.size() > 0)
		{
			int nextPos = (prePosition + 1) % mMaxPlayer;
			auto iterNext = mPlayerPositionList.find(nextPos);
			while (true)
			{
				if (iterNext->second != NULL)
				{
					iterNext->second->getCharacterData()->mBanker = true;
					// 通知房间中的所有玩家有庄家变化
					if (mPlayState == MPS_WAITING)
					{
						notifyAllPlayerBanker(iterNext->second->getGUID());
					}
					break;
				}
				else
				{
					if (++iterNext == mPlayerPositionList.end())
					{
						iterNext = mPlayerPositionList.begin();
					}
				}
			}
		}
	}

	// 清空玩家的房间ID,庄家标记,麻将数据
	data->mRoomID = INVALID_INT_ID;
	data->mBanker = false;
	data->mPosition = -1;
	player->clearMahjong();
}

void Room::chooseContinueGame(CharacterPlayer* player, bool continueGame)
{
	// 通知房间中的其他玩家
	mPlayerChooseList.insert(player, continueGame);
	// 清空麻将数据
	player->clearMahjong();
	// 如果所有玩家都已经做出选择了,则设置为等待游戏开始状态
	if (mPlayerChooseList.size() == mMaxPlayer)
	{
		setMahjongState(MPS_WAITING);
	}
}

void Room::notifyDiceDone(CHAR_GUID playerGUID)
{
	auto iterPlayer = mPlayerList.find(playerGUID);
	if (iterPlayer == mPlayerList.end())
	{
		return;
	}
	++mDiceDoneCount;
}

void Room::notifyPlayerDrop(CharacterPlayer* player, MAHJONG mah)
{
	// 正常游戏过程中,玩家打了一张牌后,判断其他玩家是否有碰或者杠,该下家摸牌
	if (mPlayState == MPS_NORMAL_GAMING)
	{
		// 判断其他玩家是否可以碰或者杠
		bool hasAction = false;
		auto iter = mPlayerList.begin();
		auto iterEnd = mPlayerList.end();
		FOR_STL (mPlayerList, ; iter != iterEnd; ++iter)
		{
			if (iter->second != player)
			{
				txVector<MahjongAction*> checkActionList;
				CharacterData* data = iter->second->getCharacterData();
				// 是否可胡
				if (ServerUtility::canHu(data->mHandIn, mah))
				{
					auto huList = ServerUtility::generateHuType(data->mHandIn, mah, data->mPengGangList, false, false);
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
			int nextPosition = (player->getCharacterData()->mPosition + 1) % MAX_PLAYER;
			CharacterPlayer* nextPlayer = getMemberByPosition(nextPosition);
			MAHJONG mahjong = requestGet();
			if (mahjong != M_MAX)
			{
				playerGetMahjong(mahjong, nextPlayer);
				notifyPlayerGet(nextPlayer, mahjong);
			}
			// 牌已经摸完了,则本局为平局
			else
			{
				txMap<CharacterPlayer*, HuInfo*> temp;
				endGame(temp);
			}
		}
	}
}

void Room::notifyPlayerGet(CharacterPlayer* player, MAHJONG mah)
{
	// 判断是否可胡或者可杠
	CharacterData* data = player->getCharacterData();
	txVector<MahjongAction*> actionList;
	// 是否是花牌
	if (ServerUtility::isHua(mah))
	{
		// 刚摸的牌是放到末尾的,所以下标是最后一个
		playerShowHua(player, data->mHandIn.size() - 1, mah);
		// 然后再摸一张牌
		MAHJONG mahjong = requestGet();
		if (mahjong != M_MAX)
		{
			playerGetMahjong(mahjong, player);
			notifyPlayerGet(player, mahjong);
		}
		// 牌已经摸完了,则本局为平局
		else
		{
			txMap<CharacterPlayer*, HuInfo*> temp;
			endGame(temp);
		}
	}
	// 是否可胡
	else if (ServerUtility::canHu(data->mHandIn))
	{
		txVector<HU_TYPE> huList = ServerUtility::generateHuType(data->mHandIn, mah, data->mPengGangList, true, true);
		MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_HU, player, player, mah, huList);
		actionList.push_back(action);
	}
	// 是否可以杠
	else
	{
		// 摸完牌后自己手里有四张相同的牌,可以杠,注意,这里杠的牌不一定是摸的牌,可能是自己手里原本就有四张
		MAHJONG gangMahjong = ServerUtility::canGang(data->mHandIn);
		if (gangMahjong != M_MAX)
		{
			MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_GANG, player, player, gangMahjong);
			actionList.push_back(action);
		}
		// 摸了一张自己碰的牌,可以开杠
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

void Room::playerConfirmAction(CharacterPlayer* player, ACTION_TYPE type)
{
	auto iterWait = mWaitList.find(player);
	if (iterWait == mWaitList.end())
	{
		LOG_ERROR("player has no action : name : %s, action : %d", player->getName().c_str(), type);
		return;
	}
	MahjongAction* action = NULL;
	auto& actionList = iterWait->second->mActionList;
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
		auto iter = mWaitList.begin();
		auto iterEnd = mWaitList.end();
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
		auto iter = mWaitList.begin();
		auto iterEnd = mWaitList.end();
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
				// 玩家杠了一张牌以后需要再摸一张
				MAHJONG mahjong = requestGet();
				if (mahjong != M_MAX)
				{
					playerGetMahjong(mahjong, info->mPlayer);
					notifyPlayerGet(info->mPlayer, mahjong);
				}
				// 牌已经摸完了,则本局为平局
				else
				{
					txMap<CharacterPlayer*, HuInfo*> temp;
					endGame(temp);
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
					int nextPosition = (info->mDroppedPlayer->getCharacterData()->mPosition + 1) % mMaxPlayer;
					CharacterPlayer* nextPlayer = getMemberByPosition(nextPosition);
					MAHJONG mahjong = requestGet();
					if (mahjong != M_MAX)
					{
						playerGetMahjong(mahjong, nextPlayer);
						notifyPlayerGet(nextPlayer, mahjong);
					}
					// 牌已经摸完了,则本局为平局
					else
					{
						txMap<CharacterPlayer*, HuInfo*> temp;
						endGame(temp);
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
	// 计算胡牌后下一局的庄家
	// 只有一个人胡,则胡牌的是庄家
	if (huPlayerList.size() == 1)
	{
		huPlayerList.begin()->first->getCharacterData()->mBanker = true;
		auto iter0 = mPlayerPositionList.begin();
		auto iterEnd0 = mPlayerPositionList.end();
		FOR_STL(mPlayerPositionList, ; iter0 != iterEnd0; ++iter0)
		{
			bool isBanker = (huPlayerList.begin()->first == iter0->second);
			iter0->second->getCharacterData()->mBanker = isBanker;
		}
		END_FOR_STL(mPlayerPositionList);
	}
	// 多个人胡,则点炮的人是庄家
	else if (huPlayerList.size() > 1)
	{
		auto iter0 = mPlayerPositionList.begin();
		auto iterEnd0 = mPlayerPositionList.end();
		FOR_STL(mPlayerPositionList, ; iter0 != iterEnd0; ++iter0)
		{
			bool isBanker = (huPlayerList.begin()->second->mDroppedPlayer == iter0->second);
			iter0->second->getCharacterData()->mBanker = isBanker;
		}
		END_FOR_STL(mPlayerPositionList);
	}
	// 通知所有玩家本局结束,计算所有玩家的输赢,暂时不计算杠的牌
	txMap<CharacterPlayer*, int> moneyDeltaList;
	auto iter = mPlayerPositionList.begin();
	auto iterEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, int i = 0; iter != iterEnd; (++iter, ++i))
	{
		moneyDeltaList.insert(iter->second, 0);
	}
	END_FOR_STL(mPlayerPositionList);
	auto iterHu = huPlayerList.begin();
	auto iterHuEnd = huPlayerList.end();
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
			auto iterMoney = moneyDeltaList.begin();
			auto iterMoneyEnd = moneyDeltaList.end();
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
void Room::askPlayerAction(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah, const txVector<MahjongAction*>& actionList)
{
	if (actionList.size() == 0)
	{
		LOG_ERROR("has no action");
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

CharacterPlayer* Room::getMember(CHAR_GUID playerID)
{
	return mPlayerList.tryGet(playerID, NULL);
}

CharacterPlayer* Room::getMemberByPosition(CHAR_GUID playerID)
{
	return mPlayerPositionList.tryGet(playerID, NULL);
}

void Room::addPlayer(CharacterPlayer* player)
{
	int playerID = player->getGUID();
	CharacterData* data = player->getCharacterData();
	mPlayerList.insert(playerID, player);
	data->mPosition = -1;
	// 找到一个空的位置,将玩家设置到该位置上
	auto iterPosition = mPlayerPositionList.begin();
	auto iterPositionEnd = mPlayerPositionList.end();
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
		LOG_ERROR("can not find an available position!");
	}
}

void Room::removePlayer(CharacterPlayer* player)
{
	CharacterData* data = player->getCharacterData();
	mPlayerList.tryErase(data->mGUID);
	auto iterPosition = mPlayerPositionList.find(data->mPosition);
	if (iterPosition != mPlayerPositionList.end())
	{
		if (iterPosition->second == player)
		{
			iterPosition->second = NULL;
		}
		else
		{
			LOG_ERROR("player not match position!");
		}
	}
	data->mPosition = -1;
}

void Room::setMahjongState(MAHJONG_PLAY_STATE state)
{
	mPlayState = state;
	// 进入等待流程时,就是开始新一局的麻将游戏,需要重置麻将系统的数据
	if (mPlayState == MPS_WAITING)
	{
		reset();
	}
	// 开始掷骰子
	else if (mPlayState == MPS_DICE)
	{
		;
	}
	// 开始拿牌时,需要重置麻将池
	else if (mPlayState == MPS_GET_START)
	{
		resetMahjongPool(true, true);
		// 判断当前谁是庄家
		auto iter = mPlayerPositionList.begin();
		auto iterEnd = mPlayerPositionList.end();
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
			LOG_ERROR("not find banker!");
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
		auto iter = mWaitList.begin();
		auto iterEnd = mWaitList.end();
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

void Room::requestDrop(CharacterPlayer* player, int index)
{
	MAHJONG mahjong = player->getCharacterData()->mHandIn[index];
	CommandCharacterDrop* cmdDrop = NEW_CMD_INFO(cmdDrop);
	cmdDrop->mIndex = index;
	cmdDrop->mMahjong = mahjong;
	mCommandSystem->pushCommand(cmdDrop, player);

	// 通知其他玩家
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerDrop* cmd = NEW_CMD_INFO(cmd);
			cmd->mIndex = index;
			cmd->mPlayerGUID = player->getGUID();
			cmd->mMahjong = mahjong;
			mCommandSystem->pushCommand(cmd, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::reset()
{
	mMahjongPool.clear();
	mWaitList.clear();
	mPlayerChooseList.clear();
	mBankerPos = -1;
	mCurAssignPos = -1;
	mDiceDoneCount = 0;
}

void Room::resetMahjongPool(bool feng, int hua)
{
	for (int i = 0; i < MAX_MAHJONG_COUNT; ++i)
	{
		for (int j = 0; j < MAX_SINGLE_COUNT; ++j)
		{
			mMahjongPool.push_back((MAHJONG)i);
		}
	}
	if (feng)
	{
		// 加入风牌
		for (int i = 0; i < MAX_FENG_COUNT; ++i)
		{
			for (int j = 0; j < MAX_SINGLE_COUNT; ++j)
			{
				mMahjongPool.push_back((MAHJONG)(M_FENG_DONG + i));
			}
		}
	}
	if (hua)
	{
		// 加入8张花牌
		for (int i = 0; i < MAX_HUA_COUNT; ++i)
		{
			mMahjongPool.push_back((MAHJONG)(M_HUA_CHUN + i));
		}
	}
	
	// 打乱麻将的顺序
	// 复制一份麻将池,然后从中随机取出放入麻将池中,直到取完
	int mahjongCount = mMahjongPool.size();
	txVector<MAHJONG> tempPool = mMahjongPool;
	FOR_STL(mMahjongPool, int i = 0; i < mahjongCount; ++i)
	{
		int randIndex = MathUtility::randomInt(0, tempPool.size() - 1);
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

bool Room::isAllPlayerReady()
{
	bool allReady = true;
	auto iter = mPlayerList.begin();
	auto iterEnd = mPlayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (!iter->second->getCharacterData()->mReady)
		{
			allReady = false;
			break;
		}
	}
	return allReady;
}

void Room::notifyAllPlayerGetStartDone()
{
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyGetStartDone* cmd = NEW_CMD_INFO(cmd);
		mCommandSystem->pushCommand(cmd, iterPlayer->second);
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::notifyAllPlayerBanker(CHAR_GUID banker)
{
	auto iterPlayer = mPlayerList.begin();
	auto iterPlayerEnd = mPlayerList.end();
	FOR_STL(mPlayerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyBanker* cmdNotifyBanker = NEW_CMD_INFO(cmdNotifyBanker);
		cmdNotifyBanker->mBankerID = banker;
		mCommandSystem->pushCommand(cmdNotifyBanker, iterPlayer->second);
	}
	END_FOR_STL(mPlayerList);
}

void Room::notifyAllPlayerDiceDone()
{
	auto iterPlayer = mPlayerList.begin();
	auto iterPlayerEnd = mPlayerList.end();
	FOR_STL(mPlayerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyDiceDone* cmd = NEW_CMD_INFO(cmd);
		mCommandSystem->pushCommand(cmd, iterPlayer->second);
	}
	END_FOR_STL(mPlayerList);
}

void Room::notifyAllPlayerMahjongEnd(txMap<CharacterPlayer*, int>& moneyDeltaList)
{
	auto iterPos = mPlayerPositionList.begin();
	auto iterPosEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPos != iterPosEnd; ++iterPos)
	{
		CommandCharacterMahjongEnd* cmd = NEW_CMD_INFO(cmd);
		cmd->mMoneyDeltaList = moneyDeltaList;
		mCommandSystem->pushCommand(cmd, iterPos->second);
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerGetStartMahjong(MAHJONG mah, CharacterPlayer* player)
{
	CommandCharacterGetStartMahjong* cmdGetStart = NEW_CMD_INFO(cmdGetStart);
	cmdGetStart->mMahjong = mah;
	mCommandSystem->pushCommand(cmdGetStart, player);
	// 通知其他玩家
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerGetStartMahjong* cmdNotifyGetStart = NEW_CMD_INFO(cmdNotifyGetStart);
			cmdNotifyGetStart->mPlayerGUID = player->getGUID();
			cmdNotifyGetStart->mMahjong = mah;
			mCommandSystem->pushCommand(cmdNotifyGetStart, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerGetMahjong(MAHJONG mah, CharacterPlayer* player)
{
	CommandCharacterGetMahjong* cmdGetStart = NEW_CMD_INFO(cmdGetStart);
	cmdGetStart->mMahjong = mah;
	mCommandSystem->pushCommand(cmdGetStart, player);
	// 通知其他玩家
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerGetMahjong* cmdNotifyGetStart = NEW_CMD_INFO(cmdNotifyGetStart);
			cmdNotifyGetStart->mPlayerGUID = player->getGUID();
			cmdNotifyGetStart->mMahjong = mah;
			mCommandSystem->pushCommand(cmdNotifyGetStart, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerReorderMahjong(CharacterPlayer* player)
{
	CommandCharacterReorderMahjong* cmd = NEW_CMD_INFO(cmd);
	mCommandSystem->pushCommand(cmd, player);

	// 通知其他玩家有玩家的牌重新排列
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerReorderMahjong* cmdOtherReorder = NEW_CMD_INFO(cmdOtherReorder);
			cmdOtherReorder->mPlayerGUID = player->getGUID();
			mCommandSystem->pushCommand(cmdOtherReorder, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerHu(txMap<CharacterPlayer*, HuInfo*>& huInfoList)
{
	txVector<std::pair<CharacterPlayer*, txVector<HU_TYPE>>> huList;
	auto iterInfo = huInfoList.begin();
	auto iterInfoEnd = huInfoList.end();
	FOR_STL(huInfoList, ; iterInfo != iterInfoEnd; ++iterInfo)
	{
		huList.push_back(std::make_pair(iterInfo->first, iterInfo->second->mHuList));
	}
	END_FOR_STL(huInfoList);
	// 通知所有玩家
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterPlayerHu* cmdOtherHu = NEW_CMD_INFO(cmdOtherHu);
		cmdOtherHu->mHuList = huList;
		mCommandSystem->pushCommand(cmdOtherHu, iterPlayer->second);
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerGang(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah)
{
	CommandCharacterGang* cmdGang = NEW_CMD_INFO(cmdGang);
	cmdGang->mMahjong = mah;
	cmdGang->mDroppedPlayer = droppedPlayer;
	mCommandSystem->pushCommand(cmdGang, player);
	// 通知其他玩家
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerGang* cmdOtherGang = NEW_CMD_INFO(cmdOtherGang);
			cmdOtherGang->mOtherPlayer = player;
			cmdOtherGang->mDroppedPlayer = droppedPlayer;
			cmdOtherGang->mMahjong = mah;
			mCommandSystem->pushCommand(cmdOtherGang, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerPeng(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah)
{
	CommandCharacterPeng* cmdPeng = NEW_CMD_INFO(cmdPeng);
	cmdPeng->mMahjong = mah;
	cmdPeng->mDroppedPlayer = droppedPlayer;
	mCommandSystem->pushCommand(cmdPeng, player);
	// 通知其他玩家
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerPeng* cmdOtherPeng = NEW_CMD_INFO(cmdOtherPeng);
			cmdOtherPeng->mOtherPlayer = player;
			cmdOtherPeng->mDroppedPlayer = droppedPlayer;
			cmdOtherPeng->mMahjong = mah;
			mCommandSystem->pushCommand(cmdOtherPeng, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerPass(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah)
{
	CommandCharacterPass* cmdPass = NEW_CMD_INFO(cmdPass);
	cmdPass->mDroppedPlayer = droppedPlayer;
	cmdPass->mMahjong = mah;
	mCommandSystem->pushCommand(cmdPass, player);
	// 通知其他玩家
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerPass* cmdOtherPass = NEW_CMD_INFO(cmdOtherPass);
			cmdOtherPass->mOtherPlayer = player;
			cmdOtherPass->mDroppedPlayer = droppedPlayer;
			cmdOtherPass->mMahjong = mah;
			mCommandSystem->pushCommand(cmdOtherPass, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerAskDrop(CharacterPlayer* player)
{
	CommandCharacterAskDrop* cmd = NEW_CMD_INFO(cmd);
	mCommandSystem->pushCommand(cmd, player);
	// 通知其他玩家
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerAskDrop* cmdOtherAskDrop = NEW_CMD_INFO(cmdOtherAskDrop);
			cmdOtherAskDrop->mOtherPlayer = player;
			mCommandSystem->pushCommand(cmdOtherAskDrop, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerAskAction(CharacterPlayer* player, const txVector<MahjongAction*>& actionList)
{
	CommandCharacterAskAction* cmdAskAction = NEW_CMD_INFO(cmdAskAction);
	cmdAskAction->mActionList = actionList;
	mCommandSystem->pushCommand(cmdAskAction, player);
	// 通知其他玩家
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerAskAction* cmdOtherAskAction = NEW_CMD_INFO(cmdOtherAskAction);
			cmdOtherAskAction->mOtherPlayer = player;
			mCommandSystem->pushCommand(cmdOtherAskAction, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}

void Room::playerShowHua(CharacterPlayer* player, int index, MAHJONG mah)
{
	CommandCharacterShowHua* cmdShowHua = NEW_CMD_INFO(cmdShowHua);
	cmdShowHua->mIndex = index;
	cmdShowHua->mMahjong = mah;
	mCommandSystem->pushCommand(cmdShowHua, player);
	// 通知其他玩家
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR_STL(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerShowHua* cmdOtherShowHua = NEW_CMD_INFO(cmdOtherShowHua);
			cmdOtherShowHua->mOtherPlayer = player;
			cmdOtherShowHua->mIndex = index;
			cmdOtherShowHua->mMahjong = mah;
			mCommandSystem->pushCommand(cmdOtherShowHua, iterPlayer->second);
		}
	}
	END_FOR_STL(mPlayerPositionList);
}