#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomConfirmAction::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	room->playerConfirmAction(static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(mPlayerGUID)), mAction);
}

std::string CommandRoomConfirmAction::showDebugInfo()
{
	COMMAND_DEBUG("player id : %d, action : %d", (int)mPlayerGUID, mAction);
}
