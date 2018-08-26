#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomNotifyPlayerOffline::execute()
{
	// 房间中有玩家离线,则退出该玩家
	CommandCharacterLeaveRoom* cmd = NEW_CMD_INFO(cmd);
	mCommandSystem->pushCommand(cmd, mCharacterManager->getCharacter(mOfflinePlayer));
}

std::string CommandRoomNotifyPlayerOffline::showDebugInfo()
{
	COMMAND_DEBUG("offline player id : %d", (int)mOfflinePlayer);
}
