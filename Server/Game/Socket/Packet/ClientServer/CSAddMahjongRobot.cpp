#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"
#include "CommandHeader.h"
#include "NetClient.h"
#include "CharacterManager.h"
#include "CharacterPlayer.h"
#include "RoomManager.h"
#include "Room.h"
#include "CharacterData.h"
#include "MahjongRobotManager.h"
#include "CharacterMahjongRobot.h"

void CSAddMahjongRobot::execute()
{
	Character* character = mCharacterManager->getCharacter(mClient->getCharacterGUID());
	// 查看玩家所在的房间人有没有满
	bool addRet = false;
	CharacterMahjongRobot* robot = NULL;
	Room* room = mRoomManager->getRoom(character->getCharacterData()->mRoomID);
	if (room != NULL && !room->isFull())
	{
		robot = mMahjongRobotManager->createRobot();
		CommandCharacterJoinRoom* cmdJoin = NEW_CMD_INFO(cmdJoin);
		cmdJoin->mRoomID = room->getID();
		pushCommand(cmdJoin, robot);
		addRet = true;
	}
	SCAddMahjongRobotRet* addRobotRet = NEW_PACKET(addRobotRet, PT_SC_ADD_MAHJONG_ROBOT_RET);
	addRobotRet->mResult = addRet;
	sendMessage(addRobotRet, mClient);

	// 机器人加入后默认立即准备
	if (addRet)
	{
		CommandCharacterReady* cmdReady = NEW_CMD_INFO(cmdReady);
		cmdReady->mReady = true;
		pushCommand(cmdReady, robot);
	}
}
