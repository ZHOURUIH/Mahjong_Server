#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetManagerServer.h"
#include "CommandHeader.h"
#include "NetManagerClient.h"
#include "CharacterManager.h"
#include "Character.h"

void CSReady::execute()
{
	NetManagerClient* client = mNetManagerServer->getClient(mClient);
	CommandCharacterReady cmd(CMD_PARAM);
	cmd.mReady = mReady;
	mCommandSystem->pushCommand(&cmd, mCharacterManager->getCharacter(client->getCharGUID()));
}
