#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetManagerServer.h"
#include "CommandHeader.h"
#include "NetManagerClient.h"
#include "CharacterManager.h"
#include "CharacterPlayer.h"

void CSRequestDrop::execute()
{
	NetManagerClient* client = mNetManagerServer->getClient(mClient);
	CommandRoomRequestDrop cmd(CMD_PARAM);
	cmd.mPlayerGUID = client->getCharGUID();
	cmd.mIndex = mIndex;
	mCommandSystem->pushCommand(&cmd, mCharacterManager->getCharacter(client->getCharGUID()));
}
