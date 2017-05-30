#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "RoomManager.h"
#include "Room.h"
#include "CharacterData.h"

void CommandCharacterJoinRoom::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	Room* room = mRoomManager->getRoom(mRoomID);
	room->joinRoom(player);
	// 记录下玩家当前所在的房间ID
	player->getCharacterData()->mRoomID = mRoomID;
}

std::string CommandCharacterJoinRoom::showDebugInfo()
{
	COMMAND_DEBUG("room id : %d", mRoomID);
}