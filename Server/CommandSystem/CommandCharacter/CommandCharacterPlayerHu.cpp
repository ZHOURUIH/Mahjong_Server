#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterPlayerHu::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCPlayerHu* hu = static_cast<SCPlayerHu*>(mNetServer->createPacket(PT_SC_PLAYER_HU));
	hu->setHuList(mHuList);
	hu->setHuPlayer(mHuPlayerList);
	mNetServer->sendMessage(hu, player->getClientGUID());
}

std::string CommandCharacterPlayerHu::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}