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

void CSBackToMahjongHall::execute()
{
	Character* character = mCharacterManager->getCharacter(mClient->getCharGUID());
	CommandCharacterContinueGame* cmdContinue = NEW_CMD_INFO(cmdContinue);
	cmdContinue->mContinue = false;
	pushCommand(cmdContinue, character);
}
