#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"
#include "CommandHeader.h"
#include "NetClient.h"
#include "CharacterManager.h"
#include "CharacterPlayer.h"
#include "RoomManager.h"
#include "Room.h"
#include "CharacterData.h"

void CSRequestDrop::execute()
{
	Character* character = mCharacterManager->getCharacter(mClient->getCharacterGUID());
	CommandRoomRequestDrop* cmd = NEW_CMD_INFO(cmd);
	cmd->mPlayerGUID = character->getGUID();
	cmd->mIndex = mIndex;
	pushCommand(cmd, mRoomManager->getRoom(character->getCharacterData()->mRoomID));
}
