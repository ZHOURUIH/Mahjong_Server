#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"
#include "CommandHeader.h"
#include "NetClient.h"
#include "CharacterManager.h"
#include "Character.h"
#include "CharacterData.h"
#include "RoomManager.h"

void CSReady::execute()
{
	Character* character = mCharacterManager->getCharacter(mClient->getCharGUID());
	Room* room = mRoomManager->getRoom(character->getCharacterData()->mRoomID);
	CommandRoomPlayerReady* cmd = NEW_CMD_INFO(cmd);
	cmd->mReady = mReady;
	cmd->mPlayerGUID = character->getGUID();
	pushCommand(cmd, room);
}
