#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomPlayerLeave::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 将玩家从房间中移除
	room->playerLeaveRoom(mPlayer, mNotifyOtherPlayer);
}

std::string CommandRoomPlayerLeave::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}
