#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "NetManagerServer.h"
#include "PacketHeader.h"

void CommandCharacterNotifyPlayerOffline::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 发送有玩家离线的消息
	SCPlayerOffline* playerOffline = static_cast<SCPlayerOffline*>(mNetManagerServer->createPacket(PT_SC_PLAYER_OFFLINE));
	playerOffline->mUserID = mPlayerGUID;
	mNetManagerServer->sendMessage(playerOffline, player->getClientGUID());
}

std::string CommandCharacterNotifyPlayerOffline::showDebugInfo()
{
	COMMAND_DEBUG("offline player id : %d", (int)mPlayerGUID);
}
