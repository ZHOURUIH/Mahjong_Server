#include "RoomManager.h"
#include "Room.h"
#include "GameLog.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "CharacterManager.h"
#include "CommandHeader.h"
#include "Utility.h"

int RoomManager::mRoomIDSeed = 0;
void RoomManager::update(float elapsedTime)
{
	auto iter = mRoomList.begin();
	auto iterEnd = mRoomList.end();
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

Room* RoomManager::getRoom(int id)
{
	return mRoomList.tryGet(id, NULL);
}

void RoomManager::destroyRoom(int id)
{
	auto iterRoom = mRoomList.find(id);
	if (iterRoom != mRoomList.end())
	{
		TRACE_DELETE(iterRoom->second);
		mRoomList.erase(iterRoom);
	}
}

void RoomManager::destroyAllRoom()
{
	auto iterRoom = mRoomList.begin();
	auto iterRoomEnd = mRoomList.end();
	FOR_STL(mRoomList, ; iterRoom != iterRoomEnd; ++iterRoom)
	{
		TRACE_DELETE(iterRoom->second);
	}
	END_FOR_STL(mRoomList);
	mRoomList.clear();
}