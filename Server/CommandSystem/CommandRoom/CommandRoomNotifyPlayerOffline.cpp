#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomNotifyPlayerOffline::execute()
{
	// 鎴块棿涓湁鐜╁绂荤嚎,鍒欓€€鍑鸿鐜╁
	CommandCharacterLeaveRoom* cmd = NEW_CMD(cmd);
	mCommandSystem->pushCommand(cmd, mCharacterManager->getCharacter(mOfflinePlayer));
}

std::string CommandRoomNotifyPlayerOffline::showDebugInfo()
{
	COMMAND_DEBUG("offline player id : %d", (int)mOfflinePlayer);
}
