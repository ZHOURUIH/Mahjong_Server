#ifndef _ROOM_H_
#define _ROOM_H_

#include "ServerDefine.h"
#include "ServerBase.h"
#include "txCommandReceiver.h"
#include "txUtility.h"

class Room : public ServerBase, public txCommandReceiver
{
public:
	Room(int id)
		:
		txCommandReceiver("room" + txUtility::intToString(id)),
		mID(id),
		mMaxPlayer(MAX_PLAYER),
		mMapID(~0)
	{}
	virtual ~Room(){ destroy(); }
	virtual void init(){}
	virtual void update(const float& elapsedTime){}
	void destroy(){}
	const int& getID() { return mID; }
	bool joinRoom(CharacterPlayer* player);
	bool leaveRoom(CharacterPlayer* player);
	void notifyEnterGame();
	bool notifyPlayerReady(const CHAR_GUID& playerID, const bool& ready = true);
	bool notifyPlayerContinue(const CHAR_GUID& playerID, const bool& continueGame = true);
	bool isAllPlayerReady();
	bool isAllPlayerContinue();
	CharacterPlayer* getMember(const CHAR_GUID& playerID);
	bool isRoomFull() { return (int)mPlayerList.size() >= mMaxPlayer; }
	void setMapID(const int& mapID) { mMapID = mapID; }
	const int& getMapID() { return mMapID; }
	const std::map<CHAR_GUID, ROOM_PLAYER>& getPlayerList() { return mPlayerList; }
protected:
	int mID;			// 房间ID
	int mMaxPlayer;		// 房间人数上限
	std::map<CHAR_GUID, ROOM_PLAYER> mPlayerList;	// 房间中的玩家列表
	int mMapID;			// 房间当前的地图ID
	std::map<CHAR_GUID, FINISH_DATA> mFinishList;	// 完成比赛的列表,first是发送到完成比赛消息客户端角色ID
};

#endif
