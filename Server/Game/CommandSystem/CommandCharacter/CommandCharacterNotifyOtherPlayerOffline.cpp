#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "NetServer.h"
#include "PacketHeader.h"

void CommandCharacterNotifyOtherPlayerOffline::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 发送有玩家离线的消息
	SCOtherPlayerOffline* playerOffline = NEW_PACKET(playerOffline, PT_SC_OTHER_PLAYER_OFFLINE);
	playerOffline->mPlayerID = mPlayerGUID;
	sendMessage(playerOffline, player);
}

std::string CommandCharacterNotifyOtherPlayerOffline::showDebugInfo()
{
	COMMAND_DEBUG("offline player id : %d", (int)mPlayerGUID);
}
