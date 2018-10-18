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

void CSConfirmAction::execute()
{
	Character* character = mCharacterManager->getCharacter(mClient->getCharacterGUID());
	Room* room = mRoomManager->getRoom(character->getCharacterData()->mRoomID);
	CommandRoomConfirmAction* cmd = NEW_CMD_INFO(cmd);
	cmd->mPlayerGUID = character->getGUID();
	cmd->mAction = (ACTION_TYPE)mAction;
	pushCommand(cmd, room);
}
