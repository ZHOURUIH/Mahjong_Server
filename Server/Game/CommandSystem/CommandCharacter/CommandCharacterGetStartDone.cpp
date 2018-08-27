#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "Room.h"
#include "RoomManager.h"
#include "PacketHeader.h"
#include "NetServer.h"

void CommandCharacterGetStartDone::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	CharacterData* data = player->getCharacterData();
	if (data->mGetStartDone)
	{
		return;
	}
	data->mGetStartDone = true;
	// 通知房间有玩家已经拿完开局的牌
	Room* room = mRoomManager->getRoom(data->mRoomID);
	if (room != NULL)
	{
		CommandRoomNotifyPlayerGetStartDone* cmd = NEW_CMD_INFO(cmd);
		cmd->mPlayerGUID = data->mGUID;
		mCommandSystem->pushCommand(cmd, room);
	}
}