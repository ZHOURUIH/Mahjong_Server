﻿#ifndef _ROOM_H_
#define _ROOM_H_

#include "GameDefine.h"
#include "ServerBase.h"
#include "txCommandReceiver.h"
#include "txUtility.h"

class CharacterPlayer;
class WaitActionInfo;
class MahjongAction;
class Room : public ServerBase, public txCommandReceiver
{
public:
	Room(int id)
		:
		txCommandReceiver("room" + txUtility::intToString(id)),
		mID(id),
		mMaxPlayer(MAX_PLAYER),
		mLockRoom(false),
		mReadyCount(0),
		mDiceDoneCount(0),
		mPlayState(MPS_WAITING),
		mBankerPos(-1),
		mCurAssignPos(-1),
		mCurInterval(0.0f)
	{
		for (int i = 0; i < mMaxPlayer; ++i)
		{
			mPlayerPositionList.insert(std::make_pair(i, (CharacterPlayer*)NULL));
		}
	}
	virtual ~Room(){ destroy(); }
	virtual void init(){}
	virtual void update(const float& elapsedTime);
	void destroy(){}
	// 房间相关
	void joinRoom(CharacterPlayer* player);
	void leaveRoom(CharacterPlayer* player);
	void notifyEnterGame();
	void notifyPlayerReady(const CHAR_GUID& playerGUID, const bool& ready);
	void notifyDiceDone(const CHAR_GUID& playerGUID);
	void notifyPlayerDrop(CharacterPlayer* player, const MAHJONG& mah);
	void notifyPlayerGet(CharacterPlayer* player, const MAHJONG& mah);
	void askPlayerAction(CharacterPlayer* player, CharacterPlayer* droppedPlayer, const MAHJONG& mah, const std::vector<MahjongAction*>& actionList);
	void playerConfirmAction(CharacterPlayer* player, const ACTION_TYPE& type);
	CharacterPlayer* getMember(const CHAR_GUID& playerID);
	CharacterPlayer* getMemberByPosition(const CHAR_GUID& playerID);
	// 麻将相关
	void setMahjongState(const MAHJONG_PLAY_STATE& state);
	void requestDrop(CharacterPlayer* player, const int& index);
	void notifyAllPlayerDiceDone();
	const int& getID()												{ return mID; }
	bool isAllPlayerReady()											{ return mReadyCount == mMaxPlayer; }
	bool isAllPlayerDiceDone()										{ return mDiceDoneCount == mMaxPlayer; }
	bool isRoomFull()												{ return (int)mPlayerList.size() >= mMaxPlayer; }
	const std::map<CHAR_GUID, CharacterPlayer*>& getPlayerList()	{ return mPlayerList; }
	const bool& isRoomLocked()										{ return mLockRoom; }
protected:
	void reset();
	void addPlayer(CharacterPlayer* player);
	void removePlayer(CharacterPlayer* player);
	void resetMahjongPool();
	MAHJONG requestGet();
	void endGame(CharacterPlayer* player, const MAHJONG& mahjong, const std::vector<HU_TYPE>& huList);
	void notifyAllPlayerGetStartDone();
	void notifyAllPlayerBanker(const CHAR_GUID& banker);
	void playerGetStartMahjong(const MAHJONG& mah, CharacterPlayer* player);
	void playerGetMahjong(const MAHJONG& mah, CharacterPlayer* player);
	void playerReorderMahjong(CharacterPlayer* player);
	// 可能同时会有多个玩家可以胡牌
	void playerHu(const std::vector<CharacterPlayer*>& playerList, CharacterPlayer* droppedPlayer, const MAHJONG& mah, const std::vector<std::vector<HU_TYPE>>& huList);
	void playerGang(CharacterPlayer* player, CharacterPlayer* droppedPlayer, const MAHJONG& mah);
	void playerPeng(CharacterPlayer* player, CharacterPlayer* droppedPlayer, const MAHJONG& mah);
	void playerPass(CharacterPlayer* player, CharacterPlayer* droppedPlayer, const MAHJONG& mah);
	void playerAskDrop(CharacterPlayer* player);
	void playerAskAction(CharacterPlayer* player, const std::vector<MahjongAction*>& actionList);
protected:
	int mID;												// 房间ID
	int mMaxPlayer;											// 房间人数上限
	bool mLockRoom;											// 房间是否已锁定,房间锁定后,其他玩家不能再加入
	int mDiceDoneCount;										// 骰子掷完的人数
	int mReadyCount;										// 已准备的玩家人数
	std::map<CHAR_GUID, CharacterPlayer*> mPlayerList;		// 房间中的玩家列表
	std::map<int, CharacterPlayer*> mPlayerPositionList;	// 房间中的玩家位置列列表,列表长度固定
	MAHJONG_PLAY_STATE mPlayState;							// 当前麻将游戏的状态
	std::vector<MAHJONG> mMahjongPool;						// 当前麻将池中的麻将
	int mBankerPos;											// 本局庄家的位置
	int mCurAssignPos;										// 开局发牌时当前发到牌的玩家的位置
	float mCurInterval;										// 当前间隔时间计时
	std::map<CharacterPlayer*, WaitActionInfo*> mWaitList;	// 等待列表
};

#endif