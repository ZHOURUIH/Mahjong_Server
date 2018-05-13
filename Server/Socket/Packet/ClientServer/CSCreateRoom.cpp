#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"
#include "CommandHeader.h"
#include "RoomManager.h"
#include "Room.h"
#include "CharacterManager.h"
#include "Character.h"
#include "NetClient.h"

void CSCreateRoom::execute()
{
	// 发送命令创建房间
	Room* room = NULL;
	CommandRoomManagerCreateRoom* cmdCreate = NEW_CMD(cmdCreate);
	cmdCreate->mRoomPtr = &room;
	mCommandSystem->pushCommand(cmdCreate, mRoomManager);

	// 创建完毕后立即发送消息
	SCCreateRoomRet* packet = static_cast<SCCreateRoomRet*>(mNetServer->createPacket(PT_SC_CREATE_ROOM_RET));
	if (room != NULL)
	{
		packet->mResult = 1;
		packet->mRoomID = room->getID();

		// 将玩家加入房间
		NetClient* client = mNetServer->getClient(mClient);
		CommandCharacterJoinRoom* cmdJoin = NEW_CMD(cmdJoin);
		cmdJoin->mRoomID = room->getID();
		mCommandSystem->pushCommand(cmdJoin, mCharacterManager->getCharacter(client->getCharGUID()));
	}
	else
	{
		packet->mResult = 0;
		packet->mRoomID = 0;
	}
	mNetServer->sendMessage(packet, mClient);
}
