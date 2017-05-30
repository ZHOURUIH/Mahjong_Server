#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomPlayerLeave::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 一旦有玩家离开房间,则使所有玩家都退出房间
	// 此处不能使用引用
	std::map<CHAR_GUID, ROOM_PLAYER> playerList = room->getPlayerList();
	std::map<CHAR_GUID, ROOM_PLAYER>::iterator iterPlayer = playerList.begin();
	std::map<CHAR_GUID, ROOM_PLAYER>::iterator iterPlayerEnd = playerList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterLeaveRoom cmdLeave(COMMAND_PARAM);
		mCommandSystem->pushCommand(&cmdLeave, iterPlayer->second.mPlayer);
	}
}

std::string CommandRoomPlayerLeave::showDebugInfo()
{
	COMMAND_DEBUG("player id : %d", (int)mPlayerGUID);
}
