﻿#include "PacketHeader.h"
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
	NetClient* client = mNetServer->getClient(mClient);
	Character* character = mCharacterManager->getCharacter(client->getCharGUID());
	CommandCharacterContinueGame* cmdContinue = NEW_CMD_INFO(cmdContinue);
	cmdContinue->mContinue = false;
	mCommandSystem->pushCommand(cmdContinue, character);
}
