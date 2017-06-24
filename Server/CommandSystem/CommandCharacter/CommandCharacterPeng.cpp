#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterPeng::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->pengMahjong(mMahjong);
	SCPlayerPeng* peng = static_cast<SCPlayerPeng*>(mNetManagerServer->createPacket(PT_SC_PLAYER_PENG));
	peng->mMahjong = mMahjong;
	peng->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	mNetManagerServer->sendMessage(peng, player->getClientGUID());
}

std::string CommandCharacterPeng::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}