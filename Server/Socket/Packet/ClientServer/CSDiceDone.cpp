#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"
#include "CommandHeader.h"
#include "NetClient.h"
#include "CharacterManager.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "RoomManager.h"
#include "Room.h"

void CSDiceDone::execute()
{
	NetClient* client = mNetServer->getClient(mClient);
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(client->getCharGUID()));
	Room* room = mRoomManager->getRoom(player->getCharacterData()->mRoomID);
	CommandRoomNotifyDiceDone cmd(CMD_PARAM);
	cmd.mPlayerGUID = player->getGUID();
	mCommandSystem->pushCommand(&cmd, room);
}
