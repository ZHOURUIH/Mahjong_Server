#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterGang::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->gangMahjong(mMahjong, mDroppedPlayer);
	SCPlayerGang* gang = static_cast<SCPlayerGang*>(mNetManagerServer->createPacket(PT_SC_PLAYER_GANG));
	gang->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	gang->mMahjong = mMahjong;
	mNetManagerServer->sendMessage(gang, player->getClientGUID());
}

std::string CommandCharacterGang::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}