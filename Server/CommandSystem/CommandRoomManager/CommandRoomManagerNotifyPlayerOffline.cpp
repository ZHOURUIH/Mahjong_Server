#include "CommandHeader.h"
#include "RoomManager.h"

void CommandRoomManagerNotifyPlayerOffline::execute()
{
	RoomManager* roomManager = static_cast<RoomManager*>(mReceiver);
	// 如果玩家离线,则取消玩家等待
	roomManager->cancelMatch(mPlayerID);
}

std::string CommandRoomManagerNotifyPlayerOffline::showDebugInfo()
{
	COMMAND_DEBUG("player id : %d", (int)mPlayerID);
}