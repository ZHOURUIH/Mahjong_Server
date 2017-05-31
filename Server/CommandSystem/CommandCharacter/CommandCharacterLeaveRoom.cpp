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
	// 清空玩家的房间ID
	player->getCharacterData()->mRoomID = -1;
	// 通知房间有玩家离开
	CommandRoomNotifyPlayerLeave cmdLeave(CMD_PARAM);
	cmdLeave.mPlayerGUID = player->getGUID();
	mCommandSystem->pushCommand(&cmdLeave, room);
}

std::string CommandCharacterLeaveRoom::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}