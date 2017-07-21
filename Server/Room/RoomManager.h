#ifndef _ROOM_MANAGER_H_
#define _ROOM_MANAGER_H_

#include "ServerDefine.h"
#include "ServerBase.h"
#include "txCommandReceiver.h"

class Room;
class CharacterPlayer;
class RoomManager : public ServerBase, public txCommandReceiver
{
public:
	RoomManager()
		:
		txCommandReceiver(TOSTRING(RoomManager))
	{}
	virtual ~RoomManager(){ destroy(); }
	virtual void init(){}
	virtual void update(const float& elapsedTime);
	void destroy(){}
	Room* createRoom();
	Room* getRoom(const int& id);
	void destroyRoom(const int& id);
	void destroyAllRoom();
	const txMap<int, Room*>& getRoomList() { return mRoomList; }
protected:
	txMap<int, Room*> mRoomList;
	static int mRoomIDSeed;
	txMap<CHAR_GUID, CharacterPlayer*> mWaitPlayerMap;		// 等待匹配的玩家列表,用于查找
	txVector<CharacterPlayer*>	mWaitPlayerList;	// 等待匹配的玩家列表,保存着玩家之间的先后顺序
};

#endif