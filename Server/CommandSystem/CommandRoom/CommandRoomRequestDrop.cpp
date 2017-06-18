#include "CommandHeader.h"
#include "Room.h"
#include "CharacterManager.h"
#include "CharacterPlayer.h"

void CommandRoomRequestDrop::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	room->requestDrop(static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(mPlayerGUID)), mIndex);
}

std::string CommandRoomRequestDrop::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}