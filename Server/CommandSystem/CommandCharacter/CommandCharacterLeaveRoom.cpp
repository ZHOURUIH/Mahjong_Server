#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "Room.h"
#include "RoomManager.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"

void CommandCharacterLeaveRoom::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCLeaveRoomRet* leaveRet = static_cast<SCLeaveRoomRet*>(mNetManagerServer->createPacket(PT_SC_LEAVE_ROOM_RET));
	Room* room = mRoomManager->getRoom(player->getCharacterData()->mRoomID);
	if (room != NULL)
	{
		// 清空玩家的房间ID
		player->getCharacterData()->mRoomID = INVALID_ID;
		// 通知房间有玩家离开
		CommandRoomNotifyPlayerLeave cmdLeave(CMD_PARAM);
		cmdLeave.mPlayerGUID = player->getGUID();
		mCommandSystem->pushCommand(&cmdLeave, room);
		leaveRet->mResult = true;
	}
	else
	{
		leaveRet->mResult = false;
	}
	// 发送消息通知客户端离开房间的结果
	mNetManagerServer->sendMessage(leaveRet, player->getClientGUID());
}

std::string CommandCharacterLeaveRoom::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}