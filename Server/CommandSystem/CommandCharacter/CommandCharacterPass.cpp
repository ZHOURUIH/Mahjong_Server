#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterPass::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCPlayerPass* pass = static_cast<SCPlayerPass*>(mNetServer->createPacket(PT_SC_PLAYER_PASS));
	pass->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	pass->mMahjong = mMahjong;
	mNetServer->sendMessage(pass, player->getClientGUID());
}

std::string CommandCharacterPass::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}