#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomPlayerContinueGame::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 玩家选择是否继续游戏
	room->playerContinueGame(mPlayer, mContinue);
}

std::string CommandRoomPlayerContinueGame::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}
