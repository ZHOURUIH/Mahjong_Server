#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyGetStartDone::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCNotifyGetStartDone* getStartDone = static_cast<SCNotifyGetStartDone*>(mNetManagerServer->createPacket(PT_SC_NOTIFY_GET_START_DONE));
	mNetManagerServer->sendMessage(getStartDone, player->getClientGUID());
}

std::string CommandCharacterNotifyGetStartDone::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}