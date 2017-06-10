#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "NetManagerServer.h"
#include "PacketHeader.h"

void CommandCharacterNotifyOtherPlayerReady::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 发送有玩家准备的消息
	SCOtherPlayerReady* playerReady = static_cast<SCOtherPlayerReady*>(mNetManagerServer->createPacket(PT_SC_OTHER_PLAYER_READY));
	playerReady->mPlayerGUID = mPlayerGUID;
	playerReady->mReady = mReady;
	mNetManagerServer->sendMessage(playerReady, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerReady::showDebugInfo()
{
	COMMAND_DEBUG("ready : %s, player id : %d", mReady ? "true" : "false", (int)mPlayerGUID);
}
