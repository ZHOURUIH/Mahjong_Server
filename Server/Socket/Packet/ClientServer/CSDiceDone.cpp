#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetManagerServer.h"
#include "CommandHeader.h"
#include "NetManagerClient.h"
#include "CharacterManager.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "RoomManager.h"
#include "Room.h"

void CSDiceDone::execute()
{
	NetManagerClient* client = mNetManagerServer->getClient(mClient);
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(client->getCharGUID()));
	Room* room = mRoomManager->getRoom(player->getCharacterData()->mRoomID);
	CommandRoomNotifyDiceDone cmd(CMD_PARAM);
	cmd.mPlayerGUID = player->getGUID();
	mCommandSystem->pushCommand(&cmd, room);
}
