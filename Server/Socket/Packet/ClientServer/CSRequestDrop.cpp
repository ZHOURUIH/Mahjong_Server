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

void CSRequestDrop::execute()
{
	NetManagerClient* client = mNetManagerServer->getClient(mClient);
	Character* character = mCharacterManager->getCharacter(client->getCharGUID());
	CommandRoomRequestDrop cmd(CMD_PARAM);
	cmd.mPlayerGUID = client->getCharGUID();
	cmd.mIndex = mIndex;
	mCommandSystem->pushCommand(&cmd, mRoomManager->getRoom(character->getCharacterData()->mRoomID));
}
