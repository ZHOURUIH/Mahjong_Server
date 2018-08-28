﻿#include "CommandHeader.h"
#include "CharacterManager.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "RoomManager.h"
#include "Room.h"
#include "MatchSystem.h"

void CommandCharacterManagerNotifyPlayerOffline::execute()
{
	CharacterManager* characterManager = static_cast<CharacterManager*>(mReceiver);
	CharacterPlayer* offlinePlayer = static_cast<CharacterPlayer*>(characterManager->getCharacter(mPlayerID));
	if (offlinePlayer != NULL)
	{
		Room* room = mRoomManager->getRoom(offlinePlayer->getCharacterData()->mRoomID);
		if (room != NULL)
		{
			// 通知角色同房间中的其他玩家有玩家离线
			txMap<CHAR_GUID, CharacterPlayer*>& playerList = room->getPlayerList();
			auto iterPlayer = playerList.begin();
			auto iterPlayerEnd = playerList.end();
			FOR(playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
			{
				// 已经离线的玩家不作通知
				if (iterPlayer->first != mPlayerID)
				{
					CommandCharacterNotifyOtherPlayerOffline* cmdPlayerOffline = NEW_CMD_INFO(cmdPlayerOffline);
					cmdPlayerOffline->mPlayerGUID = mPlayerID;
					mCommandSystem->pushCommand(cmdPlayerOffline, iterPlayer->second);
				}
			}
			END(playerList);
			// 通知房间有玩家离线
			CommandRoomNotifyPlayerOffline* cmdRoomOffline = NEW_CMD_INFO(cmdRoomOffline);
			cmdRoomOffline->mOfflinePlayer = mPlayerID;
			mCommandSystem->pushCommand(cmdRoomOffline, room);
		}
		// 通知房间管理器有玩家离线
		mMatchSystem->notifyPlayerOffline(offlinePlayer);
	}

	// 将角色销毁
	CommandCharacterManagerDestroyCharacter* cmdDestroy = NEW_CMD_INFO(cmdDestroy);
	cmdDestroy->mGUID = mPlayerID;
	mCommandSystem->pushCommand(cmdDestroy, characterManager);
}

std::string CommandCharacterManagerNotifyPlayerOffline::showDebugInfo()
{
	COMMAND_DEBUG("player id : %d", (int)mPlayerID);
}
