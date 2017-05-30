#include "Room.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "CommandHeader.h"

bool Room::joinRoom(CharacterPlayer* player)
{
	if (isRoomFull())
	{
		return false;
	}
	if (player == NULL)
	{
		return false;
	}
	int playerID = player->getGUID();
	if (getMember(playerID) != NULL)
	{
		GAME_ERROR("error : there is a player id : %d in room! can not join!", playerID);
		return false;
	}
	mPlayerList.insert(std::make_pair(playerID, ROOM_PLAYER(player, false, false)));
	return true;
}

bool Room::leaveRoom(CharacterPlayer* player)
{
	if (player == NULL)
	{
		return false;
	}
	std::map<CHAR_GUID, ROOM_PLAYER>::iterator iterPlayer = mPlayerList.find(player->getGUID());
	if (iterPlayer != mPlayerList.end())
	{
		mPlayerList.erase(iterPlayer);
	}
	return true;
}

void Room::notifyEnterGame()
{
	mFinishList.clear();
}

bool Room::notifyPlayerReady(const CHAR_GUID& playerID, const bool& ready)
{
	std::map<CHAR_GUID, ROOM_PLAYER>::iterator iterPlayer = mPlayerList.find(playerID);
	if (iterPlayer == mPlayerList.end())
	{
		return false;
	}
	if (iterPlayer->second.mReady == ready)
	{
		return false;
	}
	iterPlayer->second.mReady = ready;
	return true;
}

bool Room::notifyPlayerContinue(const CHAR_GUID& playerID, const bool& continueGame)
{
	std::map<CHAR_GUID, ROOM_PLAYER>::iterator iterPlayer = mPlayerList.find(playerID);
	if (iterPlayer == mPlayerList.end())
	{
		return false;
	}
	if (iterPlayer->second.mContinueFlag == continueGame)
	{
		return false;
	}
	iterPlayer->second.mContinueFlag = continueGame;
	return true;
}

bool Room::isAllPlayerReady()
{
	std::map<CHAR_GUID, ROOM_PLAYER>::iterator iterPlayer = mPlayerList.begin();
	std::map<CHAR_GUID, ROOM_PLAYER>::iterator iterPlayerEnd = mPlayerList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (!iterPlayer->second.mReady)
		{
			return false;
		}
	}
	// 玩家数量大于0,并且所有玩家已经准备完毕
	return mPlayerList.size() > 0;
}

bool Room::isAllPlayerContinue()
{
	std::map<CHAR_GUID, ROOM_PLAYER>::iterator iterPlayer = mPlayerList.begin();
	std::map<CHAR_GUID, ROOM_PLAYER>::iterator iterPlayerEnd = mPlayerList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (!iterPlayer->second.mContinueFlag)
		{
			return false;
		}
	}
	// 玩家数量大于0,并且所有玩家都继续匹配
	return mPlayerList.size() > 0;
}

CharacterPlayer* Room::getMember(const CHAR_GUID& playerID)
{
	std::map<CHAR_GUID, ROOM_PLAYER>::iterator iterPlayer = mPlayerList.find(playerID);
	if (iterPlayer != mPlayerList.end())
	{
		return iterPlayer->second.mPlayer;
	}
	return NULL;
}