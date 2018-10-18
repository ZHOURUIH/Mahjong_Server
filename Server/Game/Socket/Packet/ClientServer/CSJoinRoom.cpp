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
	CommandCharacterJoinRoom* cmdJoin = NEW_CMD_INFO(cmdJoin);
	cmdJoin->mRoomID = mRoomID;
	pushCommand(cmdJoin, mCharacterManager->getCharacter(mClient->getCharacterGUID()));
}
