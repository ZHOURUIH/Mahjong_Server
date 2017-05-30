#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"

void CommandRoomNotifyPlayerOffline::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 房间中有玩家离线,退出所有玩家
	// 此处不能使用引用
	std::map<CHAR_GUID, ROOM_PLAYER> playerList = room->getPlayerList();
	std::map<CHAR_GUID, ROOM_PLAYER>::iterator iterPlayer = playerList.begin();
	std::map<CHAR_GUID, ROOM_PLAYER>::iterator iterPlayerEnd = playerList.end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		CommandCharacterLeaveRoom cmdLeave(COMMAND_PARAM);
		mCommandSystem->pushCommand(&cmdLeave, iterPlayer->second.mPlayer);
	}
	// 并且销毁房间
	CommandRoomManagerDestroyRoom* cmdDestroyRoom = txCommand::createDelayCommand<CommandRoomManagerDestroyRoom>(COMMAND_PARAM);
	cmdDestroyRoom->mRoomID = room->getID();
	mCommandSystem->pushDelayCommand(cmdDestroyRoom, mRoomManager);
}

std::string CommandRoomNotifyPlayerOffline::showDebugInfo()
{
	COMMAND_DEBUG("offline player id : %d", (int)mOfflinePlayer);
}
