#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterHu::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCPlayerHu* hu = static_cast<SCPlayerHu*>(mNetManagerServer->createPacket(PT_SC_PLAYER_HU));
	hu->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	hu->mMahjong = mMahjong;
	hu->setHuList(mHuList);
	mNetManagerServer->sendMessage(hu, player->getClientGUID());
}

std::string CommandCharacterHu::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}