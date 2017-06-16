#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterAskDrop::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCAskDrop* getStartDone = static_cast<SCAskDrop*>(mNetManagerServer->createPacket(PT_SC_ASK_DROP));
	mNetManagerServer->sendMessage(getStartDone, player->getClientGUID());
}

std::string CommandCharacterAskDrop::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}