#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "Room.h"
#include "RoomManager.h"
#include "PacketHeader.h"
#include "NetServer.h"

void CommandCharacterReady::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	CharacterData* data = player->getCharacterData();
	if (data->mReady == mReady)
	{
		return;
	}
	data->mReady = mReady;
	// 发消息通知客户端
	SCReadyRet* readyRet = NEW_PACKET(readyRet, PT_SC_READY_RET);
	readyRet->mReady = mReady;
	sendMessage(readyRet, player);
}

std::string CommandCharacterReady::showDebugInfo()
{
	COMMAND_DEBUG("ready : %s", mReady ? "true" : "false");
}