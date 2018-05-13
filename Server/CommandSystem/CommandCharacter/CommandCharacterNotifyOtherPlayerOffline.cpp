#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "NetServer.h"
#include "PacketHeader.h"

void CommandCharacterNotifyOtherPlayerOffline::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 鍙戦€佹湁鐜╁绂荤嚎鐨勬秷鎭?
	SCOtherPlayerOffline* playerOffline = static_cast<SCOtherPlayerOffline*>(mNetServer->createPacket(PT_SC_OTHER_PLAYER_OFFLINE));
	playerOffline->mPlayerID = mPlayerGUID;
	mNetServer->sendMessage(playerOffline, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerOffline::showDebugInfo()
{
	COMMAND_DEBUG("offline player id : %d", (int)mPlayerGUID);
}
