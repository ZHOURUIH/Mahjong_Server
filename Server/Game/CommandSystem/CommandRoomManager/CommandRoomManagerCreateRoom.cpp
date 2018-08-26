#include "CommandHeader.h"
#include "RoomManager.h"

void CommandRoomManagerCreateRoom::execute()
{
	RoomManager* roomManager = static_cast<RoomManager*>(mReceiver);
	Room* room = roomManager->createRoom();
	if (mRoomPtr != NULL)
	{
		*mRoomPtr = room;
	}
}

std::string CommandRoomManagerCreateRoom::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}