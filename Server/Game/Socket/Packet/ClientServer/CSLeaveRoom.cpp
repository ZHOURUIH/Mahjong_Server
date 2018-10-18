#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"
#include "CommandHeader.h"
#include "NetClient.h"
#include "CharacterManager.h"
#include "Character.h"
#include "CharacterData.h"
#include "RoomManager.h"

void CSLeaveRoom::execute()
{
	Character* character = mCharacterManager->getCharacter(mClient->getCharacterGUID());
	Room* room = mRoomManager->getRoom(character->getCharacterData()->mRoomID);
	CommandRoomPlayerLeave* cmd = NEW_CMD_INFO(cmd);
	pushCommand(cmd, character);
}