#ifndef _ROOM_H_
#define _ROOM_H_

#include "GameDefine.h"
#include "ServerBase.h"
#include "txCommandReceiver.h"

class CharacterPlayer;
class WaitActionInfo;
class MahjongAction;
class HuInfo;
class Room : public txCommandReceiver
{
public:
	Room(const int& id);
	virtual ~Room(){ destroy(); }
	virtual void init(){}
	virtual void update(float elapsedTime);
	void destroy(){}
	// 房间相关
	void joinRoom(CharacterPlayer* player);
	void playerLeaveRoom(CharacterPlayer* player, bool notifyOtherPlayer);
	void playerContinueGame(CharacterPlayer* player, bool continueGame);
	void playerConfirmAction(CharacterPlayer* player, ACTION_TYPE type);
	// 麻将相关
	// 生成所有玩家开局拿的麻将
	void generateStartMahjong(txVector<CHAR_GUID>& playerIDList, txVector<txVector<MAHJONG>>& handInMahjong, txVector<txVector<MAHJONG>>& huaMahjong);
	void setMahjongState(MAHJONG_PLAY_STATE state);
	void requestDrop(CharacterPlayer* player, int index);
	void allPlayerGetStartDone();
	void playerReady(CharacterPlayer* player, bool ready);
	// 获取状态
	//--------------------------------------------------------------------------------------------------------------------------------
	int getID()											{ return mID; }
	bool isFull()										{ return (int)mPlayerList.size() >= mMaxPlayer; }
	txMap<CHAR_GUID, CharacterPlayer*>& getPlayerList()	{ return mPlayerList; }
	bool isPublic()										{ return mPublicRoom; }
	txMap<CharacterPlayer*, bool>& getPlayerChooseList(){ return mPlayerChooseList; }
	MAHJONG_PLAY_STATE getMahjongState()				{ return mPlayState; }
	int getMaxPlayerCount()								{ return mMaxPlayer;}
	int getBankerPos()									{ return mBankerPos;}
	bool isAllPlayerReady();
	bool isAllPlayerGetStartDone();
	CharacterPlayer* getBanker() { return mPlayerPositionList[mBankerPos]; }
	CharacterPlayer* getMember(CHAR_GUID playerID);
	CharacterPlayer* getMemberByPosition(CHAR_GUID playerID);
protected:
	void requestMahjong(CharacterPlayer* player);
	void reset();
	void clearWaitList();
	void setBanker(CharacterPlayer* player);
	void addPlayer(CharacterPlayer* player);
	void removePlayer(CharacterPlayer* player);
	void resetMahjongPool(bool feng, int hua);
	MAHJONG requestGet();
	void endGame(txMap<CharacterPlayer*, HuInfo*>& huPlayerList);
	void notifyPlayerDropped(CharacterPlayer* player, MAHJONG mah);
	void notifyPlayerGet(CharacterPlayer* player, MAHJONG mah);
	void askPlayerAction(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah, const txVector<MahjongAction*>& actionList);
	CharacterPlayer* findNextPlayer(int position);
	//--------------------------------------------------------------------------------------------------------
	// 玩家行为
	void playerGetHua(CharacterPlayer* player, MAHJONG hua);
	void playerAskAction(CharacterPlayer* player, const txVector<MahjongAction*>& actionList);
	void playerAskDrop(CharacterPlayer* player);
	void playerGang(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah);
	void playerPeng(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah);
	void playerPass(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah);
	void playerReorderMahjong(CharacterPlayer* player);
	void playerGetMahjong(CharacterPlayer* player, MAHJONG mah);
	void playerDrop(CharacterPlayer* player, int index, MAHJONG mah);
	//---------------------------------------------------------------------------------------------------------
	// 通知所有玩家
	void notifyAllPlayerOtherJoinRoom(CharacterPlayer* player);
	void notifyAllPlayerContinueGame(CharacterPlayer* player);
	void notifyAllPlayerOtherLeaveRoom(CharacterPlayer* player);
	void notifyAllPlayerReady(CharacterPlayer* player, bool ready);
	void notifyAllPlayerDrop(CharacterPlayer* player, int index, MAHJONG mah);
	void notifyAllPlayerGetStartDone();
	void notifyAllPlayerBanker(CHAR_GUID banker);
	void notifyAllPlayerMahjongEnd(txMap<CharacterPlayer*, int>& moneyDeltaList);
	void notifyAllPlayerGetMahjong(CharacterPlayer* player, MAHJONG mah);
	void notifyAllPlayerReorderMahjong(CharacterPlayer* player);
	void notifyAllPlayerHu(txMap<CharacterPlayer*, HuInfo*>& huInfoList);
	void notifyAllPlayerGang(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah);
	void notifyAllPlayerPeng(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah);
	void notifyAllPlayerPass(CharacterPlayer* player, CharacterPlayer* droppedPlayer, MAHJONG mah);
	void notifyAllPlayerAskDrop(CharacterPlayer* player);
	void notifyAllPlayerGetHua(CharacterPlayer* player, MAHJONG mah);
	void notifyAllPlayerAskAction(CharacterPlayer* player, const txVector<MahjongAction*>& actionList);
protected:
	int mID;											// 房间ID
	int mMaxPlayer;										// 房间人数上限
	bool mPublicRoom;									// 房间是否公开,公开的房间可以显示到房间列表中,不公开的房间只能通过输入房间号进入
	txMap<CHAR_GUID, CharacterPlayer*> mPlayerList;		// 房间中的玩家列表
	txMap<int, CharacterPlayer*> mPlayerPositionList;	// 房间中的玩家位置列列表,列表长度固定
	MAHJONG_PLAY_STATE mPlayState;						// 当前麻将游戏的状态
	txVector<MAHJONG> mMahjongPool;						// 当前麻将池中的麻将
	int mBankerPos;										// 本局庄家的位置
	int mCurAssignPos;									// 开局发牌时当前发到牌的玩家的位置
	float mCurInterval;									// 当前间隔时间计时
	txMap<CharacterPlayer*, WaitActionInfo*> mWaitList;	// 等待列表
	txMap<CharacterPlayer*, bool> mPlayerChooseList;	// 玩家选择继续游戏还是离开房间
};

#endif