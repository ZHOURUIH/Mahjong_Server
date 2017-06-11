#include "Room.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "CommandHeader.h"

void Room::joinRoom(CharacterPlayer* player)
{
	CharacterData* data = player->getCharacterData();
	// 第一个加入房间的玩家为庄家
	data->mBanker = (mPlayerList.size() == 0);
	// 加入房间的玩家列表,并且在其中设置玩家的位置
	addPlayer(player);
}

void Room::leaveRoom(CharacterPlayer* player)
{
	CharacterData* data = player->getCharacterData();
	int prePosition = data->mPosition;
	removePlayer(player);
	// 如果是已经准备的玩家离开房间,则需要减少已经准备的玩家计数
	if (data->mReady)
	{
		--mReadyCount;
	}
	// 如果是庄家离开了房间
	if (data->mBanker)
	{
		// 找到下一个玩家设置为庄家
		if (mPlayerList.size() > 0)
		{
			int nextPos = (prePosition + 1) % mMaxPlayer;
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
						CommandCharacterNotifyBanker cmdNotifyBanker(CMD_PARAM);
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

void Room::notifyPlayerReady(const CHAR_GUID& playerGUID, const bool& ready)
{
	std::map<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = mPlayerList.find(playerGUID);
	if (iterPlayer == mPlayerList.end())
	{
		return;
	}
	if (ready)
	{
		++mReadyCount;
	}
	else
	{
		--mReadyCount;
	}
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
	data->mPosition = -1;
	// 找到一个空的位置,将玩家设置到该位置上
	std::map<int, CharacterPlayer*>::iterator iterPosition = mPlayerPositionList.begin();
	std::map<int, CharacterPlayer*>::iterator iterPositionEnd = mPlayerPositionList.end();
	for (; iterPosition != iterPositionEnd; ++iterPosition)
	{
		if (iterPosition->second == NULL)
		{
			iterPosition->second = player;
			data->mPosition = iterPosition->first;
			break;
		}
	}
	if (data->mPosition == -1)
	{
		GAME_ERROR("can not find an available position!");
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
		if (iterPosition->second == player)
		{
			iterPosition->second = NULL;
		}
		else
		{
			GAME_ERROR("player not match position!");
		}
	}
	data->mPosition = -1;
}