#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterMahjongEnd::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCNotifyMahjongEnd* mahjongEnd = static_cast<SCNotifyMahjongEnd*>(mNetManagerServer->createPacket(PT_SC_NOTIFY_MAHJONG_END));
	mNetManagerServer->sendMessage(mahjongEnd, player->getClientGUID());
}

std::string CommandCharacterMahjongEnd::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}