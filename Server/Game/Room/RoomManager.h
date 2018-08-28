#ifndef _ROOM_MANAGER_H_
#define _ROOM_MANAGER_H_

#include "FrameComponent.h"

class Room;
class CharacterPlayer;
class RoomManager : public FrameComponent
{
public:
	RoomManager(const std::string& name)
		:FrameComponent(name){}
	virtual ~RoomManager(){ destroy(); }
	virtual void init(){}
	virtual void update(float elapsedTime);
	void destroy(){}
	Room* createRoom();
	Room* getRoom(int id);
	void getNotFullPublicRoomList(txVector<Room*>& roomList);
	void destroyRoom(int id);
	void destroyAllRoom();
	txMap<int, Room*>& getRoomList() { return mRoomList; }
protected:
	txMap<int, Room*> mRoomList;
	static int mRoomIDSeed;
};

#endif