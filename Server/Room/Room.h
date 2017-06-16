#ifndef _ROOM_H_
#define _ROOM_H_

#include "ServerDefine.h"
#include "ServerBase.h"
#include "txCommandReceiver.h"
#include "txUtility.h"
#include "MahjongAction.h"

class CharacterPlayer;
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
	CharacterPlayer* getMember(const CHAR_GUID& playerID);
	// 麻将相关
	void setMahjongState(const MAHJONG_PLAY_STATE& state);
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
	void notifyOtherPlayerGetStartMahjong(CharacterPlayer* exceptPlayer, MAHJONG mah);
	void notifyAlPlayerGetStartDone();
	void notifyOtherPlayerReorder(CharacterPlayer* exceptPlayer);
	void notifyAllPlayerBanker(CHAR_GUID banker);
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