#include "CommandHeader.h"
#include "CharacterManager.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "RoomManager.h"
#include "Room.h"

void CommandCharacterManagerNotifyPlayerOffline::execute()
{
	CharacterManager* characterManager = static_cast<CharacterManager*>(mReceiver);
	// 通知角色同房间中有玩家离线
	CharacterPlayer* offlinePlayer = static_cast<CharacterPlayer*>(characterManager->getCharacter(mPlayerID));
	if (offlinePlayer != NULL)
	{
		Room* room = mRoomManager->getRoom(offlinePlayer->getCharacterData()->mRoomID);
		if (room != NULL)
		{
			std::map<CHAR_GUID, ROOM_PLAYER>::const_iterator iterPlayer = room->getPlayerList().begin();
			std::map<CHAR_GUID, ROOM_PLAYER>::const_iterator iterPlayerEnd = room->getPlayerList().end();
			for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
			{
				// 已经离线的玩家不作通知
				if (iterPlayer->first != mPlayerID)
				{
					CommandCharacterNotifyPlayerOffline cmdPlayerOffline(COMMAND_PARAM);
					cmdPlayerOffline.mPlayerGUID = mPlayerID;
					mCommandSystem->pushCommand(&cmdPlayerOffline, iterPlayer->second.mPlayer);
				}
			}
			// 通知房间有玩家离线
			CommandRoomNotifyPlayerOffline cmdRoomOffline(COMMAND_PARAM);
			cmdRoomOffline.mOfflinePlayer = mPlayerID;
			mCommandSystem->pushCommand(&cmdRoomOffline, room);
		}
	}

	// 通知房间管理器玩家离线
	CommandRoomManagerNotifyPlayerOffline cmdNotifyRoomManagerOffline(COMMAND_PARAM);
	cmdNotifyRoomManagerOffline.mPlayerID = mPlayerID;
	mCommandSystem->pushCommand(&cmdNotifyRoomManagerOffline, mRoomManager);

	// 将角色销毁
	CommandCharacterManagerDestroyCharacter cmdDestroy(COMMAND_PARAM);
	cmdDestroy.mGUID = mPlayerID;
	mCommandSystem->pushCommand(&cmdDestroy, characterManager);
}

std::string CommandCharacterManagerNotifyPlayerOffline::showDebugInfo()
{
	COMMAND_DEBUG("player id : %d", (int)mPlayerID);
}
