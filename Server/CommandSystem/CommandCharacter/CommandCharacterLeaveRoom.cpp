#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "Room.h"
#include "RoomManager.h"
#include "PacketHeader.h"

void CommandCharacterLeaveRoom::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	Room* room = mRoomManager->getRoom(player->getCharacterData()->mRoomID);
	player->getCharacterData()->mRoomID = -1;
	if (room != NULL)
	{
		// 从房间中移除
		room->leaveRoom(player);
	}
}

std::string CommandCharacterLeaveRoom::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}