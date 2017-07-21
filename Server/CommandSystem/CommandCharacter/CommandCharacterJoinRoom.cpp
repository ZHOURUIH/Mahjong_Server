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
			CommandRoomNotifyPlayerJoin cmdJoin(CMD_PARAM);
			cmdJoin.mPlayerGUID = player->getGUID();
			mCommandSystem->pushCommand(&cmdJoin, room);
			joinRet->mResult = cmdJoin.mResult;
		}
		else
		{
			joinRet->mResult = JRR_NO_ROOM;
		}
	}
	// 加入成功则设置消息中的房间号,玩家在房间中的位置,玩家的房间号
	if (joinRet->mResult == JRR_SUCC)
	{
		joinRet->mRoomID = mRoomID;
		joinRet->mServerPosition = player->getCharacterData()->mPosition;
		joinRet->mBanker = player->getCharacterData()->mBanker;
		player->getCharacterData()->mRoomID = mRoomID;
	}
	// 否则设置消息中的房间号为无效值
	else
	{
		joinRet->mRoomID = INVALID_ID;
	}
	// 因为要保证消息发送的顺序,并且在消息发送后会被自动销毁,所以在发送消息之前判断是否成功
	JOIN_ROOM_RESULT result = (JOIN_ROOM_RESULT)joinRet->mResult;
	mNetManagerServer->sendMessage(joinRet, player->getClientGUID());

	// 如果加入成功,则发送当前房间的所有玩家的数据
	if (result == JRR_SUCC)
	{
		// 房间需要再次获取一次
		Room* room = mRoomManager->getRoom(player->getCharacterData()->mRoomID);
		txMap<CHAR_GUID, CharacterPlayer*>& playerList = room->getPlayerList();
		txMap<CHAR_GUID, CharacterPlayer*>::iterator iter = playerList.begin();
		txMap<CHAR_GUID, CharacterPlayer*>::iterator iterEnd = playerList.end();
		FOR_STL(playerList, ; iter != iterEnd; ++iter)
		{
			// 玩家自己不再通知
			if (iter->second != player)
			{
				CommandCharacterNotifyOtherPlayerJoinRoom cmd(CMD_PARAM);
				cmd.mJoinPlayer = iter->second;
				mCommandSystem->pushCommand(&cmd, player);
			}
		}
		END_FOR_STL(playerList);
	}
}

std::string CommandCharacterJoinRoom::showDebugInfo()
{
	COMMAND_DEBUG("room id : %d", mRoomID);
}