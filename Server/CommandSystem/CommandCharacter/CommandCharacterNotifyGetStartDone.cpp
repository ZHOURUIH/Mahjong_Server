#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyGetStartDone::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCNotifyGetStartDone* getStartDone = static_cast<SCNotifyGetStartDone*>(mNetServer->createPacket(PT_SC_NOTIFY_GET_START_DONE));
	mNetServer->sendMessage(getStartDone, player->getClientGUID());
}

std::string CommandCharacterNotifyGetStartDone::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}