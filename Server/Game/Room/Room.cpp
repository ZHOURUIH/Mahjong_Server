#include "Room.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "CommandHeader.h"
#include "ServerUtility.h"
#include "WaitActionInfo.h"
#include "MahjongAction.h"
#include "HuInfo.h"
#include "Utility.h"
#include "RoomManager.h"

Room::Room(const int& id)
	:
	txCommandReceiver("room" + StringUtility::intToString(id)),
	mID(id),
	mMaxPlayer(MAX_PLAYER),
	mPublicRoom(true),
	mPlayState(MPS_NONE),
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
	;
}

void Room::joinRoom(CharacterPlayer* player)
{
	// 加入房间的玩家列表,并且在其中设置玩家的位置
	addPlayer(player);
	// 第一个加入房间的玩家为庄家
	if (mPlayerList.size() == 1)
	{
		// 当第一个玩家加入房间时,设置为等待状态
		setMahjongState(MPS_WAITING);
		setBanker(player);
	}
	else
	{
		CharacterData* data = player->getCharacterData();
		data->mBanker = false;
	}

	// 通知房间中的其他玩家有玩家加入
	notifyAllPlayerOtherJoinRoom(player);
}

void Room::playerLeaveRoom(CharacterPlayer* player, bool notifyOtherPlayer)
{
	CommandCharacterLeaveRoom* cmdLeave = NEW_CMD_INFO(cmdLeave);
	pushCommand(cmdLeave, player);

	CharacterData* data = player->getCharacterData();
	int prePosition = data->mPosition;
	removePlayer(player);
	// 如果是庄家离开了房间,找到下一个玩家设置为庄家
	if (data->mBanker)
	{
		CharacterPlayer* nextPlayer = findNextPlayer(prePosition);
		if (nextPlayer != NULL)
		{
			setBanker(nextPlayer);
		}
	}

	// 清空玩家的房间ID,庄家标记,麻将数据
	data->mRoomID = INVALID_INT_ID;
	data->mBanker = false;
	data->mPosition = -1;
	player->clearMahjong();

	// 房间中存在玩家时才不为空
	bool isEmpty = true;
	if (mPlayerList.size() > 0)
	{
		auto iter = mPlayerList.begin();
		auto iterEnd = mPlayerList.end();
		FOR(mPlayerList, ; iter != iterEnd; ++iter)
		{
			if (iter->second->getClientGUID() != INVALID_ID)
			{
				isEmpty = false;
				break;
			}
		}
		END(mPlayerList);
	}
	if (!isEmpty)
	{
		// 如果需要通知其他玩家
		if (notifyOtherPlayer)
		{
			notifyAllPlayerOtherLeaveRoom(player);
		}
	}
	// 房间已经没人,或者玩家离开后只剩机器人
	else
	{
		// 真实玩家退出时才会判断是否将全部的机器人退出去,然后销毁房间
		if (player->getType() == CT_PLAYER)
		{
			// 使全部机器人退出房间
			// 复制一份列表,避免迭代器失效
			auto tempList = mPlayerList;
			auto iter = tempList.begin();
			auto iterEnd = tempList.end();
			FOR(tempList, ; iter != iterEnd; ++iter)
			{
				CommandRoomPlayerLeave* leave = NEW_CMD_INFO(leave);
				leave->mNotifyOtherPlayer = false;
				pushCommand(leave, iter->second);
			}
			END(tempList);
			// 销毁房间
			CommandRoomManagerDestroyRoom* cmdDestroyRoom = NEW_CMD_INFO(cmdDestroyRoom);
			cmdDestroyRoom->mRoomID = getID();
			pushCommand(cmdDestroyRoom, mRoomManager);
		}
	}
}

void Room::playerContinueGame(CharacterPlayer* player, bool continueGame)
{
	// 玩家选择是否继续游戏
	if (continueGame)
	{
		CommandCharacterContinueGame* cmdContinue = NEW_CMD_INFO(cmdContinue);
		pushCommand(cmdContinue, player);
		// 通知其他已经选择继续游戏的玩家有玩家继续游戏
		notifyAllPlayerContinueGame(player);
	}
	// 不继续则退出房间
	else
	{
		playerLeaveRoom(player, true);
	}

	// 将玩家选择结果添加到列表中
	mPlayerChooseList.insert(player, continueGame);
	// 清空麻将数据
	player->clearMahjong();
	// 如果所有玩家都已经做出选择了,则设置为等待游戏开始状态
	if (mPlayerChooseList.size() == mMaxPlayer)
	{
		setMahjongState(MPS_WAITING);
	}
}

void Room::requestMahjong(CharacterPlayer* player)
{
	// 直到摸到一张不是花牌的才停止摸牌
	while (true)
	{
		MAHJONG mahjong = requestGet();
		if (mahjong != M_MAX)
		{
			// 摸到花牌,继续摸牌
			if (ServerUtility::isHua(mahjong))
			{
				// 刚摸的牌是放到末尾的,所以下标是最后一个
				notifyAllPlayerGetHua(player, mahjong);
			}
			// 不是花牌,则停止摸牌
			else
			{
				playerGetMahjong(player, mahjong);
				notifyPlayerGet(player, mahjong);
				break;
			}
		}
		// 牌已经摸完了,则本局为平局
		else
		{
			txMap<CharacterPlayer*, HuInfo*> temp;
			endGame(temp);
			break;
		}
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
	FOR(actionList, int i = 0; i < actionCount; ++i)
	{
		if (actionList[i]->mType == type)
		{
			action = actionList[i];
			break;
		}
	}
	END(actionList);
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
		FOR(mWaitList, ; iter != iterEnd; ++iter)
		{
			WaitActionInfo* waitInfo = iter->second;
			int count = waitInfo->mActionList.size();
			FOR(waitInfo->mActionList, int i = 0; i < count; ++i)
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
			END(waitInfo->mActionList);
		}
		END(mWaitList);
		notifyAllPlayerHu(huPlayerList);
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
		FOR(mWaitList, ; iter != iterEnd; ++iter)
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
		END(mWaitList);
		// 如果全部玩家都已经确认操作了,允许优先级最高的操作进行
		if (allConfirm)
		{
			// 游戏状态设置为正常游戏
			setMahjongState(MPS_NORMAL_GAMING);
			// 先获得信息,因为在设置状态时会将列表清空
			WaitActionInfo* info = mWaitList[highestActionPlayer];
			// 杠牌
			if (highestAction->mType == AT_GANG)
			{
				playerGang(info->mPlayer, info->mDroppedPlayer, info->mMahjong);
				// 玩家杠了一张牌以后需要再摸一张
				requestMahjong(info->mPlayer);
			}
			// 碰牌
			else if (highestAction->mType == AT_PENG)
			{
				playerPeng(info->mPlayer, info->mDroppedPlayer, info->mMahjong);
				playerAskDrop(info->mPlayer);
			}
			// 不选择任何操作,过这张牌
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
					requestMahjong(nextPlayer);
				}
			}
			// 清空等待信息列表
			clearWaitList();
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
		FOR(mPlayerPositionList, ; iter0 != iterEnd0; ++iter0)
		{
			bool isBanker = (huPlayerList.begin()->first == iter0->second);
			iter0->second->getCharacterData()->mBanker = isBanker;
		}
		END(mPlayerPositionList);
	}
	// 多个人胡,则点炮的人是庄家
	else if (huPlayerList.size() > 1)
	{
		auto iter0 = mPlayerPositionList.begin();
		auto iterEnd0 = mPlayerPositionList.end();
		FOR(mPlayerPositionList, ; iter0 != iterEnd0; ++iter0)
		{
			bool isBanker = (huPlayerList.begin()->second->mDroppedPlayer == iter0->second);
			iter0->second->getCharacterData()->mBanker = isBanker;
		}
		END(mPlayerPositionList);
	}
	// 通知所有玩家本局结束,计算所有玩家的输赢,暂时不计算杠的牌
	txMap<CharacterPlayer*, int> moneyDeltaList;
	auto iter = mPlayerPositionList.begin();
	auto iterEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, int i = 0; iter != iterEnd; (++iter, ++i))
	{
		moneyDeltaList.insert(iter->second, 0);
	}
	END(mPlayerPositionList);
	auto iterHu = huPlayerList.begin();
	auto iterHuEnd = huPlayerList.end();
	FOR(huPlayerList, ; iterHu != iterHuEnd; ++iterHu)
	{
		int huCount = iterHu->second->mHuList.size();
		int multiple = 0;
		FOR(iterHu->second->mHuList, int i = 0; i < huCount; ++i)
		{
			multiple += ServerUtility::getHuMultiple(iterHu->second->mHuList[i]);
		}
		END(iterHu->second->mHuList);
		int money = 100 * huCount;
		// 自摸,所有人都一样
		if (iterHu->second->mDroppedPlayer == iterHu->second->mHuPlayer)
		{
			auto iterMoney = moneyDeltaList.begin();
			auto iterMoneyEnd = moneyDeltaList.end();
			FOR(moneyDeltaList, ; iterMoney != iterMoneyEnd; ++iterMoney)
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
			END(moneyDeltaList);
		}
		// 别人点的炮
		else
		{
			moneyDeltaList[iterHu->first] += money;
			moneyDeltaList[iterHu->second->mDroppedPlayer] -= money;
		}
	}
	END(huPlayerList);
	notifyAllPlayerMahjongEnd(moneyDeltaList);
}

CharacterPlayer* Room::getMember(CHAR_GUID playerID)
{
	return mPlayerList.tryGet(playerID, NULL);
}

CharacterPlayer* Room::getMemberByPosition(CHAR_GUID playerID)
{
	return mPlayerPositionList.tryGet(playerID, NULL);
}

void Room::setBanker(CharacterPlayer* player)
{
	player->getCharacterData()->mBanker = true;
	// 通知房间中的所有玩家有庄家变化
	if (mPlayState == MPS_WAITING)
	{
		notifyAllPlayerBanker(player->getGUID());
	}
	mBankerPos = player->getCharacterData()->mPosition;
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
	FOR(mPlayerPositionList, ; iterPosition != iterPositionEnd; ++iterPosition)
	{
		if (iterPosition->second == NULL)
		{
			iterPosition->second = player;
			data->mPosition = iterPosition->first;
			break;
		}
	}
	END(mPlayerPositionList);
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

void Room::generateStartMahjong(txVector<CHAR_GUID>& playerIDList, txVector<txVector<MAHJONG>>& handInMahjong, txVector<txVector<MAHJONG>>& huaMahjong)
{
	if (mBankerPos == -1)
	{
		return;
	}
	// 需要从庄家开始发牌
	int curCount = 0;
	int curPos = mBankerPos;
	while (true)
	{
		playerIDList.push_back(mPlayerPositionList[curPos]->getCharacterData()->mGUID);
		handInMahjong.push_back(txVector<MAHJONG>());
		huaMahjong.push_back(txVector<MAHJONG>());
		curPos = (curPos + 1) % mMaxPlayer;
		++curCount;
		if (curCount == mMaxPlayer)
		{
			break;
		}
	}
	mCurAssignPos = mBankerPos;
	while (true)
	{
		// 给玩家发牌,如果摸到花牌,则需要将花牌拿出来,直到摸到一张不是花牌的
		while (true)
		{
			MAHJONG curMahjong = requestGet();
			// 检测是否是花牌,如果是花牌,则需要抽出来,重新摸一张牌
			if (ServerUtility::isHua(curMahjong))
			{
				huaMahjong[mCurAssignPos].push_back(curMahjong);
			}
			else
			{
				handInMahjong[mCurAssignPos].push_back(curMahjong);
				break;
			}
		}

		// 判断是否已经拿够了
		bool isDone = false;
		int palyerHandInCount = handInMahjong[mCurAssignPos].size();
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
		if (isDone)
		{
			// 如果是庄家拿完了牌,则进入正常游戏流程
			if (mCurAssignPos == mBankerPos)
			{
				break;
			}
		}
		mCurAssignPos = (mCurAssignPos + 1) % mMaxPlayer;
	}
}

void Room::setMahjongState(MAHJONG_PLAY_STATE state)
{
	mPlayState = state;
	// 进入等待流程时,就是开始新一局的麻将游戏,需要重置麻将系统的数据
	if (mPlayState == MPS_WAITING)
	{
		reset();
	}
	// 开始拿牌时,需要重置麻将池
	else if (mPlayState == MPS_GET_START)
	{
		resetMahjongPool(true, true);
	}
	else if (mPlayState == MPS_NORMAL_GAMING)
	{
		;
	}
	else if (mPlayState == MPS_WAIT_FOR_ACTION)
	{
		;
	}
	else if (mPlayState == MPS_ENDING)
	{
		clearWaitList();
	}
}

void Room::reset()
{
	mMahjongPool.clear();
	clearWaitList();
	mPlayerChooseList.clear();
	mBankerPos = -1;
	mCurAssignPos = -1;
}

void Room::clearWaitList()
{
	auto iter = mWaitList.begin();
	auto iterEnd = mWaitList.end();
	FOR(mWaitList, ; iter != iterEnd; ++iter)
	{
		int actionCount = iter->second->mActionList.size();
		FOR(iter->second->mActionList, int i = 0; i < actionCount; ++i)
		{
			TRACE_DELETE(iter->second->mActionList[i]);
		}
		END(iter->second->mActionList);
		TRACE_DELETE(iter->second);
	}
	END(mWaitList);
	mWaitList.clear();
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
	FOR(mMahjongPool, int i = 0; i < mahjongCount; ++i)
	{
		int randIndex = MathUtility::randomInt(0, tempPool.size() - 1);
		mMahjongPool[i] = tempPool[randIndex];
		// 填充临时麻将池中的空隙
		tempPool[randIndex] = tempPool[mahjongCount - i - 1];
		tempPool.erase(tempPool.begin() + tempPool.size() - 1);
	}
	END(mMahjongPool);
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

bool Room::isAllPlayerGetStartDone()
{
	bool allGetStartDone = true;
	auto iter = mPlayerList.begin();
	auto iterEnd = mPlayerList.end();
	FOR_R (mPlayerList, ; iter != iterEnd; ++iter)
	{
		if (!iter->second->getCharacterData()->mGetStartDone)
		{
			allGetStartDone = false;
			break;
		}
	}
	END_R(mPlayerList);
	return allGetStartDone;
}

bool Room::isAllPlayerReady()
{
	bool allReady = true;
	auto iter = mPlayerList.begin();
	auto iterEnd = mPlayerList.end();
	FOR (mPlayerList, ; iter != iterEnd; ++iter)
	{
		if (!iter->second->getCharacterData()->mReady)
		{
			allReady = false;
			break;
		}
	}
	END(mPlayerList);
	return allReady;
}

void Room::allPlayerGetStartDone()
{
	notifyAllPlayerGetStartDone();
	setMahjongState(MPS_NORMAL_GAMING);
	// 通知庄家打出一张牌
	playerAskDrop(getBanker());
}

void Room::playerReady(CharacterPlayer* player, bool ready)
{
	CommandCharacterReady* cmd = NEW_CMD_INFO(cmd);
	cmd->mReady = ready;
	pushCommand(cmd, player);
	// 通知其他玩家
	notifyAllPlayerReady(player, ready);
	// 所有玩家都准备完毕后,通知房间开始游戏
	if (isFull() && isAllPlayerReady())
	{
		CommandRoomStartGame* cmd = NEW_CMD_INFO(cmd);
		pushCommand(cmd, this);
	}
}

void Room::requestDrop(CharacterPlayer* player, int index)
{
	MAHJONG mahjong = player->getCharacterData()->mHandIn[index];
	playerDrop(player, index, mahjong);
	notifyPlayerDropped(player, mahjong);
}

void Room::notifyPlayerDropped(CharacterPlayer* player, MAHJONG mah)
{
	// 正常游戏过程中,玩家打了一张牌后,判断其他玩家是否有碰或者杠,该下家摸牌
	if (mPlayState == MPS_NORMAL_GAMING)
	{
		// 判断其他玩家是否可以碰或者杠
		bool hasAction = false;
		auto iter = mPlayerList.begin();
		auto iterEnd = mPlayerList.end();
		FOR(mPlayerList, ; iter != iterEnd; ++iter)
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
		END(mPlayerList);
		// 没有人需要这张牌
		if (!hasAction)
		{
			// 该下家摸牌
			int nextPosition = (player->getCharacterData()->mPosition + 1) % MAX_PLAYER;
			CharacterPlayer* nextPlayer = getMemberByPosition(nextPosition);
			requestMahjong(nextPlayer);
		}
	}
}

void Room::notifyPlayerGet(CharacterPlayer* player, MAHJONG mah)
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
		FOR(data->mPengGangList, int i = 0; i < pengGangCount; ++i)
		{
			if (data->mPengGangList[i]->mMahjong == mah && data->mPengGangList[i]->mType == AT_PENG)
			{
				MahjongAction* action = TRACE_NEW(MahjongAction, action, AT_GANG, player, player, mah);
				actionList.push_back(action);
				break;
			}
		}
		END(data->mPengGangList)
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

CharacterPlayer* Room::findNextPlayer(int position)
{
	CharacterPlayer* nextPlayer = NULL;
	if (mPlayerList.size() > 0)
	{
		int nextPos = (position + 1) % mPlayerPositionList.size();
		auto iterNext = mPlayerPositionList.find(nextPos);
		while (true)
		{
			if (iterNext->second != NULL)
			{
				nextPlayer = iterNext->second;
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
	return nextPlayer;
}

// 玩家行为
//----------------------------------------------------------------------------------------------------------------------------
void Room::playerGetHua(CharacterPlayer* player, MAHJONG hua)
{
	// 玩家得到一张花牌
	CommandCharacterGetHua* cmdGetHua = NEW_CMD_INFO(cmdGetHua);
	cmdGetHua->mMahjong = hua;
	pushCommand(cmdGetHua, player);
	// 通知其他玩家
	notifyAllPlayerGetHua(player, hua);
}

void Room::playerAskAction(CharacterPlayer* player, const txVector<MahjongAction*>& actionList)
{
	// 通知其他玩家
	notifyAllPlayerAskAction(player, actionList);
	// 询问玩家操作
	CommandCharacterAskAction* cmdAskAction = NEW_CMD_INFO(cmdAskAction);
	cmdAskAction->mActionList = actionList;
	pushCommand(cmdAskAction, player);
}

void Room::playerAskDrop(CharacterPlayer* player)
{
	// 通知其他玩家
	notifyAllPlayerAskDrop(player);
	// 通知该玩家打出一张牌
	CommandCharacterAskDrop* cmd = NEW_CMD_INFO(cmd);
	pushCommand(cmd, player);
}

void Room::playerPass(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah)
{
	CommandCharacterPass* cmdPass = NEW_CMD_INFO(cmdPass);
	cmdPass->mDroppedPlayer = droppedPlayer;
	cmdPass->mMahjong = mah;
	pushCommand(cmdPass, player);
	// 通知其他玩家
	notifyAllPlayerPass(player, droppedPlayer, mah);
}

void Room::playerPeng(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah)
{
	CommandCharacterPeng* cmdPeng = NEW_CMD_INFO(cmdPeng);
	cmdPeng->mMahjong = mah;
	cmdPeng->mDroppedPlayer = droppedPlayer;
	pushCommand(cmdPeng, player);
	// 通知其他玩家
	notifyAllPlayerPeng(player, droppedPlayer, mah);
}

void Room::playerGang(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah)
{
	CommandCharacterGang* cmdGang = NEW_CMD_INFO(cmdGang);
	cmdGang->mMahjong = mah;
	cmdGang->mDroppedPlayer = droppedPlayer;
	pushCommand(cmdGang, player);
	// 通知其他玩家
	notifyAllPlayerGang(player, droppedPlayer, mah);
}

void Room::playerReorderMahjong(CharacterPlayer* player)
{
	CommandCharacterReorderMahjong* cmd = NEW_CMD_INFO(cmd);
	pushCommand(cmd, player);
	// 通知其他玩家有玩家的牌重新排列
	notifyAllPlayerReorderMahjong(player);
}

void Room::playerGetMahjong(CharacterPlayer* player, MAHJONG mah)
{
	CommandCharacterGetMahjong* cmdGetStart = NEW_CMD_INFO(cmdGetStart);
	cmdGetStart->mMahjong = mah;
	pushCommand(cmdGetStart, player);
	// 通知其他玩家
	notifyAllPlayerGetMahjong(player, mah);
}

void Room::playerDrop(CharacterPlayer* player, int index, MAHJONG mah)
{
	// 打出指定牌
	CommandCharacterDrop* cmdDrop = NEW_CMD_INFO(cmdDrop);
	cmdDrop->mIndex = index;
	cmdDrop->mMahjong = mah;
	pushCommand(cmdDrop, player);
	// 通知其他玩家
	notifyAllPlayerDrop(player, index, mah);
}

// 通知房间中其他玩家
//-------------------------------------------------------------------------------------------------------------------------------
void Room::notifyAllPlayerOtherJoinRoom(CharacterPlayer* player)
{
	auto iterPlayer = mPlayerList.begin();
	auto iterPlayerEnd = mPlayerList.end();
	FOR(mPlayerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerJoinRoom* cmdJoin = NEW_CMD_INFO(cmdJoin);
			cmdJoin->mJoinPlayer = player;
			pushCommand(cmdJoin, iterPlayer->second);
		}
	}
	END(mPlayerList);
}

void Room::notifyAllPlayerContinueGame(CharacterPlayer* player)
{
	auto& playerChooseList = getPlayerChooseList();
	auto iter = playerChooseList.begin();
	auto iterEnd = playerChooseList.end();
	FOR(playerChooseList, ; iter != iterEnd; ++iter)
	{
		// 只通知已经选择继续游戏的玩家
		if (iter->second)
		{
			// 通知其他玩家
			CommandCharacterNotifyOtherPlayerContinueGame* cmdOtherContinue = NEW_CMD_INFO(cmdOtherContinue);
			cmdOtherContinue->mOtherPlayer = player;
			pushCommand(cmdOtherContinue, iter->first);
			// 通知刚选择继续游戏的玩家其他玩家的信息,只有选择了继续游戏,才会发送其他玩家的选择
			CommandCharacterNotifyOtherPlayerContinueGame* cmdNotifyPlayer = NEW_CMD_INFO(cmdNotifyPlayer);
			cmdNotifyPlayer->mOtherPlayer = iter->first;
			pushCommand(cmdNotifyPlayer, player);
		}
	}
	END(playerChooseList);
}

void Room::notifyAllPlayerOtherLeaveRoom(CharacterPlayer* player)
{
	auto iterPlayer = mPlayerList.begin();
	auto iterPlayerEnd = mPlayerList.end();
	FOR(mPlayerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyOtherPlayerLeaveRoom* cmdLeave = NEW_CMD_INFO(cmdLeave);
		cmdLeave->mLeavePlayerID = player->getGUID();
		pushCommand(cmdLeave, iterPlayer->second);
	}
	END(mPlayerList);
}

void Room::notifyAllPlayerReady(CharacterPlayer* player, bool ready)
{
	// 通知房间中的其他玩家有玩家已经准备
	auto iterPlayer = mPlayerList.begin();
	auto iterPlayerEnd = mPlayerList.end();
	FOR(mPlayerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerReady* cmd = NEW_CMD_INFO(cmd);
			cmd->mPlayerGUID = player->getGUID();
			cmd->mReady = ready;
			pushCommand(cmd, iterPlayer->second);
		}
	}
	END(mPlayerList);
}
void Room::notifyAllPlayerDrop(CharacterPlayer* player, int index, MAHJONG mah)
{
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerDrop* cmd = NEW_CMD_INFO(cmd);
			cmd->mIndex = index;
			cmd->mPlayerGUID = player->getGUID();
			cmd->mMahjong = mah;
			pushCommand(cmd, iterPlayer->second);
		}
	}
	END(mPlayerPositionList);
}

void Room::notifyAllPlayerMahjongEnd(txMap<CharacterPlayer*, int>& moneyDeltaList)
{
	auto iterPos = mPlayerPositionList.begin();
	auto iterPosEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, ; iterPos != iterPosEnd; ++iterPos)
	{
		CommandCharacterMahjongEnd* cmd = NEW_CMD_INFO(cmd);
		cmd->mMoneyDeltaList = moneyDeltaList;
		pushCommand(cmd, iterPos->second);
	}
	END(mPlayerPositionList);
}

void Room::notifyAllPlayerGetStartDone()
{
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyAllGetStartDone* cmd = NEW_CMD_INFO(cmd);
		pushCommand(cmd, iterPlayer->second);
	}
	END(mPlayerPositionList);
}

void Room::notifyAllPlayerBanker(CHAR_GUID banker)
{
	auto iterPlayer = mPlayerList.begin();
	auto iterPlayerEnd = mPlayerList.end();
	FOR(mPlayerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterNotifyBanker* cmdNotifyBanker = NEW_CMD_INFO(cmdNotifyBanker);
		cmdNotifyBanker->mBankerID = banker;
		pushCommand(cmdNotifyBanker, iterPlayer->second);
	}
	END(mPlayerList);
}

void Room::notifyAllPlayerGetMahjong(CharacterPlayer* player, MAHJONG mah)
{
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerGetMahjong* cmdNotifyGetStart = NEW_CMD_INFO(cmdNotifyGetStart);
			cmdNotifyGetStart->mPlayerGUID = player->getGUID();
			cmdNotifyGetStart->mMahjong = mah;
			pushCommand(cmdNotifyGetStart, iterPlayer->second);
		}
	}
	END(mPlayerPositionList);
}

void Room::notifyAllPlayerReorderMahjong(CharacterPlayer* player)
{
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerReorderMahjong* cmdOtherReorder = NEW_CMD_INFO(cmdOtherReorder);
			cmdOtherReorder->mPlayerGUID = player->getGUID();
			pushCommand(cmdOtherReorder, iterPlayer->second);
		}
	}
	END(mPlayerPositionList);
}

void Room::notifyAllPlayerHu(txMap<CharacterPlayer*, HuInfo*>& huInfoList)
{
	// 可能同时会有多个玩家可以胡牌
	txVector<std::pair<CharacterPlayer*, txVector<HU_TYPE>>> huList;
	auto iterInfo = huInfoList.begin();
	auto iterInfoEnd = huInfoList.end();
	FOR(huInfoList, ; iterInfo != iterInfoEnd; ++iterInfo)
	{
		huList.push_back(std::make_pair(iterInfo->first, iterInfo->second->mHuList));
	}
	END(huInfoList);
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterPlayerHu* cmdOtherHu = NEW_CMD_INFO(cmdOtherHu);
		cmdOtherHu->mHuList = huList;
		pushCommand(cmdOtherHu, iterPlayer->second);
	}
	END(mPlayerPositionList);
}

void Room::notifyAllPlayerGang(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah)
{
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerGang* cmdOtherGang = NEW_CMD_INFO(cmdOtherGang);
			cmdOtherGang->mOtherPlayer = player;
			cmdOtherGang->mDroppedPlayer = droppedPlayer;
			cmdOtherGang->mMahjong = mah;
			pushCommand(cmdOtherGang, iterPlayer->second);
		}
	}
	END(mPlayerPositionList);
}

void Room::notifyAllPlayerPeng(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah)
{
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerPeng* cmdOtherPeng = NEW_CMD_INFO(cmdOtherPeng);
			cmdOtherPeng->mOtherPlayer = player;
			cmdOtherPeng->mDroppedPlayer = droppedPlayer;
			cmdOtherPeng->mMahjong = mah;
			pushCommand(cmdOtherPeng, iterPlayer->second);
		}
	}
	END(mPlayerPositionList);
}

void Room::notifyAllPlayerPass(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah)
{
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerPass* cmdOtherPass = NEW_CMD_INFO(cmdOtherPass);
			cmdOtherPass->mOtherPlayer = player;
			cmdOtherPass->mDroppedPlayer = droppedPlayer;
			cmdOtherPass->mMahjong = mah;
			pushCommand(cmdOtherPass, iterPlayer->second);
		}
	}
	END(mPlayerPositionList);
}

void Room::notifyAllPlayerAskDrop(CharacterPlayer* player)
{
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerAskDrop* cmdOtherAskDrop = NEW_CMD_INFO(cmdOtherAskDrop);
			cmdOtherAskDrop->mOtherPlayer = player;
			pushCommand(cmdOtherAskDrop, iterPlayer->second);
		}
	}
	END(mPlayerPositionList);
}

void Room::notifyAllPlayerGetHua(CharacterPlayer* player, MAHJONG mah)
{
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerGetHua* cmdOtherGetHua = NEW_CMD_INFO(cmdOtherGetHua);
			cmdOtherGetHua->mOtherPlayer = player;
			cmdOtherGetHua->mMahjong = mah;
			pushCommand(cmdOtherGetHua, iterPlayer->second);
		}
	}
	END(mPlayerPositionList);
}

void Room::notifyAllPlayerAskAction(CharacterPlayer* player, const txVector<MahjongAction*>& actionList)
{
	// 通知其他玩家
	auto iterPlayer = mPlayerPositionList.begin();
	auto iterPlayerEnd = mPlayerPositionList.end();
	FOR(mPlayerPositionList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != player)
		{
			CommandCharacterNotifyOtherPlayerAskAction* cmdOtherAskAction = NEW_CMD_INFO(cmdOtherAskAction);
			cmdOtherAskAction->mOtherPlayer = player;
			pushCommand(cmdOtherAskAction, iterPlayer->second);
		}
	}
	END(mPlayerPositionList);
}