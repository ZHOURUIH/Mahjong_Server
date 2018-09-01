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
	player->notifyLeaveRoom();

	if (player->getType() == CT_PLAYER)
	{
		// 发送消息通知客户端离开房间的结果
		SCLeaveRoomRet* leaveRet = NEW_PACKET(leaveRet, PT_SC_LEAVE_ROOM_RET);
		leaveRet->mResult = true;
		sendMessage(leaveRet, player);
	}
}