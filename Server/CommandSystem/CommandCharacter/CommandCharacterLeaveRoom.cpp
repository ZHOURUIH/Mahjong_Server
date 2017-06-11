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
	CharacterData* data = player->getCharacterData();
	SCLeaveRoomRet* leaveRet = static_cast<SCLeaveRoomRet*>(mNetManagerServer->createPacket(PT_SC_LEAVE_ROOM_RET));
	Room* room = mRoomManager->getRoom(data->mRoomID);
	if (room != NULL)
	{
		// 通知房间有玩家离开
		CommandRoomNotifyPlayerLeave cmdLeave(CMD_PARAM);
		cmdLeave.mPlayerGUID = player->getGUID();
		mCommandSystem->pushCommand(&cmdLeave, room);
		// 清空玩家的房间ID,庄家标记,准备标记
		data->mRoomID = INVALID_ID;
		data->mReady = false;
		data->mBanker = false;
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