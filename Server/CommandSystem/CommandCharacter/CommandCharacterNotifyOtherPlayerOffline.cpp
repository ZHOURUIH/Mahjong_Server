#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "NetManagerServer.h"
#include "PacketHeader.h"

void CommandCharacterNotifyOtherPlayerOffline::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 发送有玩家离线的消息
	SCOtherPlayerOffline* playerOffline = static_cast<SCOtherPlayerOffline*>(mNetManagerServer->createPacket(PT_SC_OTHER_PLAYER_OFFLINE));
	playerOffline->mPlayerID = mPlayerGUID;
	mNetManagerServer->sendMessage(playerOffline, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerOffline::showDebugInfo()
{
	COMMAND_DEBUG("offline player id : %d", (int)mPlayerGUID);
}
