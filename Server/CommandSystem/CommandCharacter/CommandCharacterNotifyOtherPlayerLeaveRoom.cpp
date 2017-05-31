#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerLeaveRoom::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 发送消息通知客户端
	SCOtherPlayerLeaveRoom* leaveRoom = static_cast<SCOtherPlayerLeaveRoom*>(mNetManagerServer->createPacket(PT_SC_OTHER_PLAYER_LEAVE_ROOM));
	leaveRoom->mPlayerGUID = mLeavePlayerID;
	mNetManagerServer->sendMessage(leaveRoom, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerLeaveRoom::showDebugInfo()
{
	COMMAND_DEBUG("player guid : %d", mLeavePlayerID);
}