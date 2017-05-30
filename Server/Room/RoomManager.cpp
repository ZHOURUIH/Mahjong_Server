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

bool RoomManager::requestMatch(CharacterPlayer* player)
{
	if (player == NULL)
	{
		return false;
	}
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterWait = mWaitPlayerMap.find(player->getGUID());
	if (iterWait != mWaitPlayerMap.end())
	{
		return false;
	}
	mWaitPlayerList.push_back(player);
	mWaitPlayerMap.insert(std::make_pair(player->getGUID(), player));
	return true;
}

bool RoomManager::requestMatch(const CHAR_GUID& playerID)
{
	Character* character = mCharacterManager->getCharacter(playerID);
	if (character != NULL && character->getType() == CT_PLAYER)
	{
		return requestMatch(static_cast<CharacterPlayer*>(character));
	}
	return false;
}

bool RoomManager::cancelMatch(CharacterPlayer* player)
{
	if (player == NULL)
	{
		return false;
	}
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterWait = mWaitPlayerMap.find(player->getGUID());
	if (iterWait == mWaitPlayerMap.end())
	{
		return false;
	}
	mWaitPlayerMap.erase(iterWait);
	int playerCount = mWaitPlayerList.size();
	for (int i = 0; i < playerCount; ++i)
	{
		if (mWaitPlayerList[i] == player)
		{
			mWaitPlayerList.erase(mWaitPlayerList.begin() + i);
			break;
		}
	}
	return true;
}

bool RoomManager::cancelMatch(const CHAR_GUID& playerID)
{
	Character* character = mCharacterManager->getCharacter(playerID);
	if (character != NULL && character->getType() == CT_PLAYER)
	{
		return cancelMatch(static_cast<CharacterPlayer*>(character));
	}
	return false;
}