#include "CommandHeader.h"
#include "RoomManager.h"

void CommandRoomManagerCreateRoom::execute()
{
	RoomManager* roomManager = static_cast<RoomManager*>(mReceiver);
	mResultRoom = roomManager->createRoom();
}

std::string CommandRoomManagerCreateRoom::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}