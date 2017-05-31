#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetManagerServer.h"
#include "CommandHeader.h"
#include "RoomManager.h"
#include "Room.h"
#include "CharacterManager.h"
#include "Character.h"
#include "NetManagerClient.h"

void CSJoinRoom::execute()
{
	NetManagerClient* client = mNetManagerServer->getClient(mClient);
	CommandCharacterJoinRoom cmdJoin(COMMAND_PARAM);
	cmdJoin.mRoomID = mRoomID;
	mCommandSystem->pushCommand(&cmdJoin, mCharacterManager->getCharacter(client->getCharGUID()));
}
