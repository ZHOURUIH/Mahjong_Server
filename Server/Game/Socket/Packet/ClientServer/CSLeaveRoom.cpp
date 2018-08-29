﻿#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"
#include "CommandHeader.h"
#include "NetClient.h"
#include "CharacterManager.h"
#include "Character.h"

void CSLeaveRoom::execute()
{
	CommandCharacterLeaveRoom* cmd = NEW_CMD_INFO(cmd);
	pushCommand(cmd, mCharacterManager->getCharacter(mClient->getCharGUID()));
}