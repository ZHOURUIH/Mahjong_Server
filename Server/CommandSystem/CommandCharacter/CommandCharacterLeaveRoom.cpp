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
		CommandRoomNotifyPlayerLeave* cmdLeave = NEW_CMD(cmdLeave);
		cmdLeave->mPlayer = player;
		mCommandSystem->pushCommand(cmdLeave, room);
	}
	SCLeaveRoomRet* leaveRet = static_cast<SCLeaveRoomRet*>(mNetServer->createPacket(PT_SC_LEAVE_ROOM_RET));
	leaveRet->mResult = (room != NULL);
	// 发送消息通知客户端离开房间的结果
	mNetServer->sendMessage(leaveRet, player->getClientGUID());
}