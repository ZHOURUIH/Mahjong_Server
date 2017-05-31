#include "RoomManager.h"
#include "Room.h"
#include "GameLog.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "CharacterManager.h"
#include "CommandHeader.h"

int RoomManager::mRoomIDSeed = 0;
void RoomManager::update(const float& elapsedTime)
{
	;
}

Room* RoomManager::createRoom()
{
	Room* room = TRACE_NEW(Room, room, mRoomIDSeed++);
	room->init();
	mRoomList.insert(std::make_pair(room->getID(), room));
	return room;
}

Room* RoomManager::getRoom(const int& id)
{
	std::map<int, Room*>::iterator iterRoom = mRoomList.find(id);
	if (iterRoom != mRoomList.end())
	{
		return iterRoom->second;
	}
	return NULL;
}

void RoomManager::destroyRoom(const int& id)
{
	std::map<int, Room*>::iterator iterRoom = mRoomList.find(id);
	if (iterRoom != mRoomList.end())
	{
		TRACE_DELETE(iterRoom->second);
		mRoomList.erase(iterRoom);
	}
}

void RoomManager::destroyAllRoom()
{
	std::map<int, Room*>::iterator iterRoom = mRoomList.begin();
	std::map<int, Room*>::iterator iterRoomEnd = mRoomList.end();
	for (; iterRoom != iterRoomEnd; ++iterRoom)
	{
		TRACE_DELETE(iterRoom->second);
	}
	mRoomList.clear();
}