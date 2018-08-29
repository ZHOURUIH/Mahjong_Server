#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "Room.h"
#include "RoomManager.h"
#include "PacketHeader.h"
#include "NetServer.h"

void CommandCharacterLeaveRoom::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	Room* room = mRoomManager->getRoom(player->getCharacterData()->mRoomID);
	if (room != NULL)
	{
		// 通知房间有玩家离开
		CommandRoomNotifyPlayerLeave* cmdLeave = NEW_CMD_INFO(cmdLeave);
		cmdLeave->mPlayer = player;
		pushCommand(cmdLeave, room);
	}
	player->notifyLeaveRoom();

	if (player->getType() == CT_PLAYER)
	{
		SCLeaveRoomRet* leaveRet = NEW_PACKET(leaveRet, PT_SC_LEAVE_ROOM_RET);
		leaveRet->mResult = (room != NULL);
		// 发送消息通知客户端离开房间的结果
		sendMessage(leaveRet, player);
	}
}