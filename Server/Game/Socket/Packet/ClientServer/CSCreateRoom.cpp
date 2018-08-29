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
	CommandRoomManagerCreateRoom* cmdCreate = NEW_CMD_INFO(cmdCreate);
	cmdCreate->mRoomPtr = &room;
	pushCommand(cmdCreate, mRoomManager);

	// 创建完毕后立即发送消息
	SCCreateRoomRet* packet = NEW_PACKET(packet, PT_SC_CREATE_ROOM_RET);
	if (room != NULL)
	{
		packet->mResult = 1;
		packet->mRoomID = room->getID();

		// 将玩家加入房间
		CommandCharacterJoinRoom* cmdJoin = NEW_CMD_INFO(cmdJoin);
		cmdJoin->mRoomID = room->getID();
		pushCommand(cmdJoin, mCharacterManager->getCharacter(mClient->getCharGUID()));
	}
	else
	{
		packet->mResult = 0;
		packet->mRoomID = 0;
	}
	sendMessage(packet, mClient);
}
