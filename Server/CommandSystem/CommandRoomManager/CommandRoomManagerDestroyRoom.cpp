#include "CommandHeader.h"
#include "RoomManager.h"

void CommandRoomManagerDestroyRoom::execute()
{
	RoomManager* roomManager = static_cast<RoomManager*>(mReceiver);
	roomManager->destroyRoom(mRoomID);
}

std::string CommandRoomManagerDestroyRoom::showDebugInfo()
{
	COMMAND_DEBUG("room id : %d", mRoomID);
}