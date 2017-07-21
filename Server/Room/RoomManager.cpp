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
	txMap<int, Room*>::iterator iter = mRoomList.begin();
	txMap<int, Room*>::iterator iterEnd = mRoomList.end();
	FOR_STL(mRoomList, ; iter != iterEnd; ++iter)
	{
		iter->second->update(elapsedTime);
	}
	END_FOR_STL(mRoomList);
}

Room* RoomManager::createRoom()
{
	Room* room = TRACE_NEW(Room, room, mRoomIDSeed++);
	room->init();
	mRoomList.insert(room->getID(), room);
	return room;
}

Room* RoomManager::getRoom(const int& id)
{
	txMap<int, Room*>::iterator iterRoom = mRoomList.find(id);
	if (iterRoom != mRoomList.end())
	{
		return iterRoom->second;
	}
	return NULL;
}

void RoomManager::destroyRoom(const int& id)
{
	txMap<int, Room*>::iterator iterRoom = mRoomList.find(id);
	if (iterRoom != mRoomList.end())
	{
		TRACE_DELETE(iterRoom->second);
		mRoomList.erase(iterRoom);
	}
}

void RoomManager::destroyAllRoom()
{
	txMap<int, Room*>::iterator iterRoom = mRoomList.begin();
	txMap<int, Room*>::iterator iterRoomEnd = mRoomList.end();
	FOR_STL(mRoomList, ; iterRoom != iterRoomEnd; ++iterRoom)
	{
		TRACE_DELETE(iterRoom->second);
	}
	END_FOR_STL(mRoomList);
	mRoomList.clear();
}