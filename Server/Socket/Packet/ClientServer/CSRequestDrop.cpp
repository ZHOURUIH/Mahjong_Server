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
	NetClient* client = mNetServer->getClient(mClient);
	Character* character = mCharacterManager->getCharacter(client->getCharGUID());
	CommandRoomRequestDrop cmd(CMD_PARAM);
	cmd.mPlayerGUID = client->getCharGUID();
	cmd.mIndex = mIndex;
	mCommandSystem->pushCommand(&cmd, mRoomManager->getRoom(character->getCharacterData()->mRoomID));
}
