#include "CommandHeader.h"
#include "Room.h"
#include "CharacterPlayer.h"

void CommandRoomPlayerReady::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 先通知房间有玩家准备好
	room->notifyPlayerReady(mPlayerGUID);
	// 所有玩家都准备完毕后,通知房间开始游戏
	if (room->isAllPlayerReady())
	{
		CommandRoomStartGame cmdStartGame(CMD_PARAM);
		mCommandSystem->pushCommand(&cmdStartGame, room);
	}
}

std::string CommandRoomPlayerReady::showDebugInfo()
{
	COMMAND_DEBUG("player is : %d", (int)mPlayerGUID);
}
