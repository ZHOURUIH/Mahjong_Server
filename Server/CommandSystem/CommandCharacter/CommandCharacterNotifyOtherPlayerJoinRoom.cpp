#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerJoinRoom::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 发送消息通知客户端
	SCOtherPlayerJoinRoom* leaveRoom = static_cast<SCOtherPlayerJoinRoom*>(mNetManagerServer->createPacket(PT_SC_OTHER_PLAYER_LEAVE_ROOM));
	leaveRoom->mPlayerGUID = mJoinPlayerID;
	mNetManagerServer->sendMessage(leaveRoom, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerJoinRoom::showDebugInfo()
{
	COMMAND_DEBUG("player guid : %d", mJoinPlayerID);
}