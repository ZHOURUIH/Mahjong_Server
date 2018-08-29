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
	// 通知房间有玩家准备
	Room* room = mRoomManager->getRoom(data->mRoomID);
	if (room != NULL)
	{
		CommandRoomNotifyPlayerReady* cmd = NEW_CMD_INFO(cmd);
		cmd->mReady = mReady;
		cmd->mPlayerGUID = data->mGUID;
		pushCommand(cmd, room);
	}
}

std::string CommandCharacterReady::showDebugInfo()
{
	COMMAND_DEBUG("ready : %s", mReady ? "true" : "false");
}