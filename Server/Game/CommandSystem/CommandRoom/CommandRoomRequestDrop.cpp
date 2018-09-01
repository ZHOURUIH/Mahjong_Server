#include "CommandHeader.h"
#include "Room.h"
#include "CharacterManager.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"

void CommandRoomRequestDrop::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(mPlayerGUID));
	MAHJONG mahjong = player->getCharacterData()->mHandIn[mIndex];
	room->requestDrop(player, mIndex);
}

std::string CommandRoomRequestDrop::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}