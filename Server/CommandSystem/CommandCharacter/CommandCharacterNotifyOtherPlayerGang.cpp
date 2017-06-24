#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerGang::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerGang* otherGang = static_cast<SCOtherPlayerGang*>(mNetManagerServer->createPacket(PT_SC_OTHER_PLAYER_GANG));
	otherGang->mOtherPlayerGUID = mOtherPlayer->getGUID();
	otherGang->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	otherGang->mMahjong = mMahjong;
	mNetManagerServer->sendMessage(otherGang, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerGang::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}