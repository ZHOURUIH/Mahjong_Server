#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterPass::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCPlayerPass* pass = static_cast<SCPlayerPass*>(mNetManagerServer->createPacket(PT_SC_PLAYER_PASS));
	pass->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	pass->mMahjong = mMahjong;
	mNetManagerServer->sendMessage(pass, player->getClientGUID());
}

std::string CommandCharacterPass::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}