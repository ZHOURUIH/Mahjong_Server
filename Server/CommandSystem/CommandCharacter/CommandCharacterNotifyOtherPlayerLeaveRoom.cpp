#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerLeaveRoom::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 发送消息通知客户端
	SCOtherPlayerLeaveRoom* leaveRoom = NetServer::createPacket(leaveRoom, PT_SC_OTHER_PLAYER_LEAVE_ROOM);
	leaveRoom->mPlayerGUID = mLeavePlayerID;
	mNetServer->sendMessage(leaveRoom, player);
}

std::string CommandCharacterNotifyOtherPlayerLeaveRoom::showDebugInfo()
{
	COMMAND_DEBUG("player guid : %d", (int)mLeavePlayerID);
}