#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomNotifyPlayerLeave::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 将玩家从房间中移除
	if (room != NULL)
	{
		room->leaveRoom(static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(mPlayerGUID)));
	}
	// 房间中还有人则通知房间中的其他玩家有玩家离开
	if (room->getPlayerList().size() > 0)
	{
		txMap<CHAR_GUID, CharacterPlayer*>& playerList = room->getPlayerList();
		txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = playerList.begin();
		txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayerEnd = playerList.end();
		FOR_STL(playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
		{
			CommandCharacterNotifyOtherPlayerLeaveRoom cmdLeave(CMD_PARAM);
			cmdLeave.mLeavePlayerID = mPlayerGUID;
			mCommandSystem->pushCommand(&cmdLeave, iterPlayer->second);
		}
		END_FOR_STL(playerList);
	}
	// 如果房间中没有人了,则销毁房间
	else
	{
		CommandRoomManagerDestroyRoom cmdDestroyRoom(CMD_PARAM);
		cmdDestroyRoom.mRoomID = room->getID();
		mCommandSystem->pushCommand(&cmdDestroyRoom, mRoomManager);
	}
}

std::string CommandRoomNotifyPlayerLeave::showDebugInfo()
{
	COMMAND_DEBUG("player id : %d", (int)mPlayerGUID);
}
