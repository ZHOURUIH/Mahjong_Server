#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterStartGame::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCStartGame* startGamePacket = static_cast<SCStartGame*>(mNetManagerServer->createPacket(PT_SC_START_GAME));
	mNetManagerServer->sendMessage(startGamePacket, player->getClientGUID());
}

std::string CommandCharacterStartGame::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}