﻿#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "RoomManager.h"
#include "Room.h"
#include "CharacterData.h"
#include "NetServer.h"
#include "PacketHeader.h"

void CommandCharacterJoinRoom::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCJoinRoomRet* joinRet = NetServer::createPacket(joinRet, PT_SC_JOIN_ROOM_RET);
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
			JOIN_ROOM_RESULT result = JRR_NO_ROOM;
			CommandRoomNotifyPlayerJoin* cmdJoin = NEW_CMD_INFO(cmdJoin);
			cmdJoin->mPlayerGUID = player->getGUID();
			cmdJoin->mResult = &result;
			mCommandSystem->pushCommand(cmdJoin, room);
			joinRet->mResult = result;
		}
		else
		{
			joinRet->mResult = JRR_NO_ROOM;
		}
	}
	// 加入成功则设置消息中的房间号,玩家在房间中的位置,玩家的房间号
	if (joinRet->mResult == JRR_SUCCESS)
	{
		joinRet->mRoomID = mRoomID;
		joinRet->mServerPosition = player->getCharacterData()->mPosition;
		joinRet->mBanker = player->getCharacterData()->mBanker;
		player->getCharacterData()->mRoomID = mRoomID;
	}
	// 否则设置消息中的房间号为无效值
	else
	{
		joinRet->mRoomID = INVALID_INT_ID;
	}
	// 因为要保证消息发送的顺序,并且在消息发送后会被自动销毁,所以在发送消息之前判断是否成功
	JOIN_ROOM_RESULT result = (JOIN_ROOM_RESULT)joinRet->mResult;
	mNetServer->sendMessage(joinRet, player);

	// 如果加入成功,则发送当前房间的所有玩家的数据
	if (result == JRR_SUCCESS)
	{
		// 房间需要再次获取一次
		Room* room = mRoomManager->getRoom(player->getCharacterData()->mRoomID);
		auto& playerList = room->getPlayerList();
		auto iter = playerList.begin();
		auto iterEnd = playerList.end();
		FOR(playerList, ; iter != iterEnd; ++iter)
		{
			// 玩家自己不再通知
			if (iter->second != player)
			{
				CommandCharacterNotifyOtherPlayerJoinRoom* cmd = NEW_CMD_INFO(cmd);
				cmd->mJoinPlayer = iter->second;
				mCommandSystem->pushCommand(cmd, player);
			}
		}
		END(playerList);
	}
}

std::string CommandCharacterJoinRoom::showDebugInfo()
{
	COMMAND_DEBUG("room id : %d", mRoomID);
}