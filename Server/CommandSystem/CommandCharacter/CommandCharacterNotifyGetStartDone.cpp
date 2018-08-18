﻿#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyGetStartDone::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCNotifyGetStartDone* getStartDone = NetServer::createPacket(getStartDone, PT_SC_NOTIFY_GET_START_DONE);
	mNetServer->sendMessage(getStartDone, player);
}