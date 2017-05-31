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
		std::map<CHAR_GUID, CharacterPlayer*>::const_iterator iterPlayer = room->getPlayerList().begin();
		std::map<CHAR_GUID, CharacterPlayer*>::const_iterator iterPlayerEnd = room->getPlayerList().end();
		for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
		{
			CommandCharacterNotifyOtherPlayerLeaveRoom cmdLeave(CMD_PARAM);
			cmdLeave.mLeavePlayerID = mPlayerGUID;
			mCommandSystem->pushCommand(&cmdLeave, iterPlayer->second);
		}
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
