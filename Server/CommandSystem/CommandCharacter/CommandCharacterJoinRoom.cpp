#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "RoomManager.h"
#include "Room.h"
#include "CharacterData.h"
#include "NetManagerServer.h"
#include "PacketHeader.h"

void CommandCharacterJoinRoom::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCJoinRoomRet* joinRet = static_cast<SCJoinRoomRet*>(mNetManagerServer->createPacket(PT_SC_JOIN_ROOM_RET));
	Room* curRoom = mRoomManager->getRoom(player->getCharacterData()->mRoomID);
	// 已经在房间中
	if (curRoom != NULL)
	{
		joinRet->mResult = JRR_PLAYER_IN_ROOM;
	}
	// 不在房间中
	else
	{
		Room* room = mRoomManager->getRoom(mRoomID);
		if (room != NULL)
		{
			CommandRoomNotifyPlayerJoin cmdJoin(COMMAND_PARAM);
			cmdJoin.mPlayerGUID = player->getGUID();
			mCommandSystem->pushCommand(&cmdJoin, room);
			// 如果加入成功,设置房间ID号,并通知客户端加入房间的结果
			if (cmdJoin.mResult == JRR_SUCC)
			{
				player->getCharacterData()->mRoomID = mRoomID;
			}
			joinRet->mResult = cmdJoin.mResult;
		}
		else
		{
			joinRet->mResult = JRR_NO_ROOM;
		}
	}
	// 加入成功则设置消息中的房间号
	if (joinRet->mResult == JRR_SUCC)
	{
		joinRet->mRoomID = mRoomID;
	}
	// 否则设置消息中的房间号为无效值
	else
	{
		joinRet->mRoomID = INVALID_ID;
	}
	mNetManagerServer->sendMessage(joinRet, player->getClientGUID());
}

std::string CommandCharacterJoinRoom::showDebugInfo()
{
	COMMAND_DEBUG("room id : %d", mRoomID);
}