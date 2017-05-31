#ifndef _ROOM_H_
#define _ROOM_H_

#include "ServerDefine.h"
#include "ServerBase.h"
#include "txCommandReceiver.h"
#include "txUtility.h"

class CharacterPlayer;
class Room : public ServerBase, public txCommandReceiver
{
public:
	Room(int id)
		:
		txCommandReceiver("room" + txUtility::intToString(id)),
		mID(id),
		mMaxPlayer(MAX_PLAYER),
		mLockRoom(false)
	{
		for (int i = 0; i < mMaxPlayer; ++i)
		{
			mPlayerPositionList.insert(std::make_pair(i, (CharacterPlayer*)NULL));
		}
	}
	virtual ~Room(){ destroy(); }
	virtual void init(){}
	virtual void update(const float& elapsedTime){}
	void destroy(){}
	const int& getID() { return mID; }
	void joinRoom(CharacterPlayer* player);
	void leaveRoom(CharacterPlayer* player);
	void notifyEnterGame();
	bool notifyPlayerReady(const CHAR_GUID& playerID, const bool& ready = true);
	bool notifyPlayerContinue(const CHAR_GUID& playerID, const bool& continueGame = true);
	bool isAllPlayerReady();
	bool isAllPlayerContinue();
	CharacterPlayer* getMember(const CHAR_GUID& playerID);
	bool isRoomFull() { return (int)mPlayerList.size() >= mMaxPlayer; }
	const std::map<CHAR_GUID, CharacterPlayer*>& getPlayerList() { return mPlayerList; }
	const bool& isRoomLocked(){ return mLockRoom; }
protected:
	void addPlayer(CharacterPlayer* player);
	void removePlayer(CharacterPlayer* player);
protected:
	int mID;			// 房间ID
	int mMaxPlayer;		// 房间人数上限
	bool mLockRoom;		// 房间是否已锁定,房间锁定后,其他玩家不能再加入
	std::map<CHAR_GUID, CharacterPlayer*> mPlayerList;				// 房间中的玩家列表
	std::map<int, CharacterPlayer*> mPlayerPositionList;	// 房间中的玩家位置列列表,列表长度固定
	std::map<CHAR_GUID, FINISH_DATA> mFinishList;				// 完成比赛的列表,first是发送到完成比赛消息客户端角色ID
};

#endif
