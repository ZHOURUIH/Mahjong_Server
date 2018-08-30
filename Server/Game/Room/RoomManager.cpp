﻿#include "RoomManager.h"
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
	// 更新所有房间
	auto iter = mRoomList.begin();
	auto iterEnd = mRoomList.end();
	FOR(mRoomList, ; iter != iterEnd; ++iter)
	{
		iter->second->update(elapsedTime);
	}
	END(mRoomList);
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

void RoomManager::getNotFullPublicRoomList(txVector<Room*>& roomList)
{
	auto iterRoom = mRoomList.begin();
	auto iterRoomEnd = mRoomList.end();
	FOR(mRoomList, ; iterRoom != iterRoomEnd; ++iterRoom)
	{
		if (!iterRoom->second->isFull() && iterRoom->second->isPublic())
		{
			roomList.push_back(iterRoom->second);
		}
	}
	END(mRoomList);
}

void RoomManager::getNotGamingPublicRoomList(txVector<Room*>& roomList)
{
	auto iterRoom = mRoomList.begin();
	auto iterRoomEnd = mRoomList.end();
	FOR(mRoomList, ; iterRoom != iterRoomEnd; ++iterRoom)
	{
		if (iterRoom->second->getMahjongState() == MPS_WAITING && iterRoom->second->isPublic())
		{
			roomList.push_back(iterRoom->second);
		}
	}
	END(mRoomList);
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
	FOR(mRoomList, ; iterRoom != iterRoomEnd; ++iterRoom)
	{
		TRACE_DELETE(iterRoom->second);
	}
	END(mRoomList);
	mRoomList.clear();
}