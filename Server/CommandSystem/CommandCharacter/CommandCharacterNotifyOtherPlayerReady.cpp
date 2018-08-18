#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "NetServer.h"
#include "PacketHeader.h"

void CommandCharacterNotifyOtherPlayerReady::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 发送有玩家准备的消息
	SCOtherPlayerReady* playerReady = NetServer::createPacket(playerReady, PT_SC_OTHER_PLAYER_READY);
	playerReady->mPlayerGUID = mPlayerGUID;
	playerReady->mReady = mReady;
	mNetServer->sendMessage(playerReady, player);
}

std::string CommandCharacterNotifyOtherPlayerReady::showDebugInfo()
{
	COMMAND_DEBUG("ready : %s, player id : %d", mReady ? "true" : "false", (int)mPlayerGUID);
}
