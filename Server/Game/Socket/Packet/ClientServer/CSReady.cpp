#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"
#include "CommandHeader.h"
#include "NetClient.h"
#include "CharacterManager.h"
#include "Character.h"

void CSReady::execute()
{
	NetClient* client = mNetServer->getClient(mClient);
	CommandCharacterReady* cmd = NEW_CMD_INFO(cmd);
	cmd->mReady = mReady;
	mCommandSystem->pushCommand(cmd, mCharacterManager->getCharacter(client->getCharGUID()));
}
