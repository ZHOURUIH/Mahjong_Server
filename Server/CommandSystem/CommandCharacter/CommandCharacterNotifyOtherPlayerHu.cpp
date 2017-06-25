#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerHu::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerHu* hu = static_cast<SCOtherPlayerHu*>(mNetManagerServer->createPacket(PT_SC_OTHER_PLAYER_HU));
	hu->mOtherPlayerGUID = mOtherPlayer->getClientGUID();
	hu->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	hu->mMahjong = mMahjong;
	hu->setHuList(mHuList);
	mNetManagerServer->sendMessage(hu, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerHu::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}