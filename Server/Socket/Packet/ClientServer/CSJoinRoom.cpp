#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"
#include "CommandHeader.h"
#include "RoomManager.h"
#include "Room.h"
#include "CharacterManager.h"
#include "Character.h"
#include "NetClient.h"

void CSJoinRoom::execute()
{
	NetClient* client = mNetServer->getClient(mClient);
	CommandCharacterJoinRoom cmdJoin(CMD_PARAM);
	cmdJoin.mRoomID = mRoomID;
	mCommandSystem->pushCommand(&cmdJoin, mCharacterManager->getCharacter(client->getCharGUID()));
}
