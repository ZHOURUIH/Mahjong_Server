#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomPlayerReady::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(mPlayerGUID));
	room->playerReady(player, mReady);
}

std::string CommandRoomPlayerReady::showDebugInfo()
{
	COMMAND_DEBUG("player id : %d, ready : %s", (int)mPlayerGUID, mReady ? "true" : "false");
}
