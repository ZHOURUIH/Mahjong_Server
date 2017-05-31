#include "Room.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "CommandHeader.h"

void Room::joinRoom(CharacterPlayer* player)
{
	CharacterData* data = player->getCharacterData();
	// 加入房间的玩家列表
	addPlayer(player);
	// 第一个加入房间的玩家为庄家
	data->mBanker = (mPlayerList.size() == 0);
	// 并且通知玩家庄家有变化
	CommandCharacterNotifyBanker cmdBanker(COMMAND_PARAM);
	cmdBanker.mBankerID = player->getGUID();
	mCommandSystem->pushCommand(&cmdBanker, player);
}

void Room::leaveRoom(CharacterPlayer* player)
{
	CharacterData* data = player->getCharacterData();
	removePlayer(player);
	// 如果是庄家离开了房间
	if (data->mBanker)
	{
		data->mBanker = false;
		// 找到下一个玩家设置为庄家
		if (mPlayerList.size() > 0)
		{
			int nextPos = (data->mPosition + 1) % mMaxPlayer;
			std::map<int, CharacterPlayer*>::iterator iterNext = mPlayerPositionList.find(nextPos);
			while (true)
			{
				if (iterNext == mPlayerPositionList.end())
				{
					iterNext = mPlayerPositionList.begin();
				}
				if (iterNext->second != NULL)
				{
					iterNext->second->getCharacterData()->mBanker = true;
					// 通知房间中的所有玩家有庄家变化
					std::map<CHAR_GUID, CharacterPlayer*>::iterator iterBanker = mPlayerList.begin();
					std::map<CHAR_GUID, CharacterPlayer*>::iterator iterBankerEnd = mPlayerList.end();
					for (; iterBanker != iterBankerEnd; ++iterBanker)
					{
						CommandCharacterNotifyBanker cmdNotifyBanker(COMMAND_PARAM);
						cmdNotifyBanker.mBankerID = iterNext->second->getGUID();
						mCommandSystem->pushCommand(&cmdNotifyBanker, iterBanker->second);
					}
					break;
				}
				else
				{
					++iterNext;
				}
			}
		}
	}
}

void Room::notifyEnterGame()
{
	mFinishList.clear();
}

bool Room::notifyPlayerReady(const CHAR_GUID& playerID, const bool& ready)
{
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.find(playerID);
	if (iterPlayer == mPlayerList.end())
	{
		return false;
	}
	if (iterPlayer->second->getCharacterData()->mReady == ready)
	{
		return false;
	}
	iterPlayer->second->getCharacterData()->mReady = ready;
	return true;
}

bool Room::isAllPlayerReady()
{
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.begin();
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayerEnd = mPlayerList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (!iterPlayer->second->getCharacterData()->mReady)
		{
			return false;
		}
	}
	// 玩家数量大于0,并且所有玩家已经准备完毕
	return mPlayerList.size() > 0;
}

CharacterPlayer* Room::getMember(const CHAR_GUID& playerID)
{
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.find(playerID);
	if (iterPlayer != mPlayerList.end())
	{
		return iterPlayer->second;
	}
	return NULL;
}

void Room::addPlayer(CharacterPlayer* player)
{
	int playerID = player->getGUID();
	CharacterData* data = player->getCharacterData();
	mPlayerList.insert(std::make_pair(playerID, player));
	std::map<int, CharacterPlayer*>::iterator iterPosition = mPlayerPositionList.find(data->mPosition);
	if (iterPosition != mPlayerPositionList.end())
	{
		iterPosition->second = player;
	}
}

void Room::removePlayer(CharacterPlayer* player)
{
	CharacterData* data = player->getCharacterData();
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.find(data->mGUID);
	if (iterPlayer != mPlayerList.end())
	{
		mPlayerList.erase(iterPlayer);
	}
	std::map<int, CharacterPlayer*>::iterator iterPosition = mPlayerPositionList.find(data->mPosition);
	if (iterPosition != mPlayerPositionList.end())
	{
		iterPosition->second = NULL;
	}
}