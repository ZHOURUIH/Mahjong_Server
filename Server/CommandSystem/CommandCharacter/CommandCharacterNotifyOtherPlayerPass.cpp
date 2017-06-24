#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerPass::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerPass* otherPass = static_cast<SCOtherPlayerPass*>(mNetManagerServer->createPacket(PT_SC_OTHER_PLAYER_PASS));
	otherPass->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	otherPass->mOtherPlayerGUID = mOtherPlayer->getGUID();
	otherPass->mMahjong = mMahjong;
	mNetManagerServer->sendMessage(otherPass, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerPass::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}