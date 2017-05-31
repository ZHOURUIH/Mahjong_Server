#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetManagerServer.h"
#include "CommandHeader.h"
#include "RoomManager.h"
#include "Room.h"
#include "CharacterManager.h"
#include "Character.h"
#include "NetManagerClient.h"

void CSCreateRoom::execute()
{
	// 发送命令创建房间
	CommandRoomManagerCreateRoom cmdCreate(CMD_PARAM);
	mCommandSystem->pushCommand(&cmdCreate, mRoomManager);

	// 创建完毕后立即发送消息
	SCCreateRoomRet* packet = static_cast<SCCreateRoomRet*>(mNetManagerServer->createPacket(PT_SC_CREATE_ROOM_RET));
	if (cmdCreate.mResultRoom != NULL)
	{
		packet->mResult = 1;
		packet->mRoomID = cmdCreate.mResultRoom->getID();

		// 将玩家加入房间
		NetManagerClient* client = mNetManagerServer->getClient(mClient);
		CommandCharacterJoinRoom cmdJoin(CMD_PARAM);
		cmdJoin.mRoomID = cmdCreate.mResultRoom->getID();
		mCommandSystem->pushCommand(&cmdJoin, mCharacterManager->getCharacter(client->getCharGUID()));
	}
	else
	{
		packet->mResult = 0;
		packet->mRoomID = 0;
	}
	mNetManagerServer->sendMessage(packet, mClient);
}
