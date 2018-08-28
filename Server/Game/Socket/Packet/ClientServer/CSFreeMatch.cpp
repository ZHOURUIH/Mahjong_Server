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
#include "MatchSystem.h"

void CSFreeMatch::execute()
{
	NetClient* client = mNetServer->getClient(mClient);
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(client->getCharGUID()));
	CommandMatchSystemRequestMatch* cmd = NEW_CMD_INFO(cmd);
	cmd->mPlayer = player;
	mCommandSystem->pushCommand(cmd, mMatchSystem);
}
