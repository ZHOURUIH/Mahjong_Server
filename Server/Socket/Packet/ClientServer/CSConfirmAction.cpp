#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetManagerServer.h"
#include "CommandHeader.h"
#include "NetManagerClient.h"
#include "CharacterManager.h"
#include "CharacterPlayer.h"
#include "RoomManager.h"
#include "Room.h"
#include "CharacterData.h"

void CSConfirmAction::execute()
{
	NetManagerClient* client = mNetManagerServer->getClient(mClient);
	Character* character = mCharacterManager->getCharacter(client->getCharGUID());
	Room* room = mRoomManager->getRoom(character->getCharacterData()->mRoomID);
	CommandRoomConfirmAction cmd(CMD_PARAM);
	cmd.mPlayerGUID = character->getGUID();
	cmd.mAction = (ACTION_TYPE)mAction;
	mCommandSystem->pushCommand(&cmd, room);
}
