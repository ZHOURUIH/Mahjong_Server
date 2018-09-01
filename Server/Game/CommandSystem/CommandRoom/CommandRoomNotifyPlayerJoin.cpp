#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomNotifyPlayerJoin::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	if (room->isFull())
	{
		setResult(JRR_FULL);
		return;
	}
	if (room->getMember(mPlayerGUID) != NULL)
	{
		setResult(JRR_PLAYER_IN_ROOM);
		return;
	}
	// 将玩家加入房间中
	CharacterPlayer* joinPlayer = static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(mPlayerGUID));
	room->joinRoom(joinPlayer);
	
	setResult(JRR_SUCCESS);
}

std::string CommandRoomNotifyPlayerJoin::showDebugInfo()
{
	COMMAND_DEBUG("player id : %d", (int)mPlayerGUID);
}
