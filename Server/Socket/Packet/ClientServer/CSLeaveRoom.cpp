#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetManagerServer.h"
#include "CommandHeader.h"
#include "NetManagerClient.h"
#include "CharacterManager.h"
#include "Character.h"

void CSLeaveRoom::execute()
{
	NetManagerClient* client = mNetManagerServer->getClient(mClient);
	CommandCharacterLeaveRoom cmd(CMD_PARAM);
	mCommandSystem->pushCommand(&cmd, mCharacterManager->getCharacter(client->getCharGUID()));
}