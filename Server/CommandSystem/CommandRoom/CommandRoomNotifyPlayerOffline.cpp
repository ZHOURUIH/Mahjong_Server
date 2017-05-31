#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomNotifyPlayerOffline::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 房间中有玩家离线,则退出该玩家,并且
	// 此处不能使用引用
	CommandCharacterLeaveRoom cmdLeave(COMMAND_PARAM);
	mCommandSystem->pushCommand(&cmdLeave, mCharacterManager->getCharacter(mOfflinePlayer));
}

std::string CommandRoomNotifyPlayerOffline::showDebugInfo()
{
	COMMAND_DEBUG("offline player id : %d", (int)mOfflinePlayer);
}
