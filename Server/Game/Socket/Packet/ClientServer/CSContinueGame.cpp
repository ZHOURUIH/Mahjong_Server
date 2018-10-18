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

void CSContinueGame::execute()
{
	CharacterPlayer* character = static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(mClient->getCharacterGUID()));
	Room* room = mRoomManager->getRoom(character->getCharacterData()->mRoomID);
	CommandRoomPlayerContinueGame* cmdContinue = NEW_CMD_INFO(cmdContinue);
	cmdContinue->mContinue = mContinue;
	cmdContinue->mPlayer = character;
	pushCommand(cmdContinue, room);
}
