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

void CSRequestRoomList::execute()
{
	txVector<Room*> roomList;
	mRoomManager->getNotGamingPublicRoomList(roomList);
	txVector<Room*> needRoomList;
	int count = roomList.size();
	int needCount = mMaxIndex - mMinIndex;
	FOR(roomList, int i = 0; i < needCount; ++i)
	{
		if (i >= count)
		{
			break;
		}
		needRoomList.push_back(roomList[i + mMinIndex]);
	}
	END(roomList)
	SCRoomList* roomListPacket = NEW_PACKET(roomListPacket, PT_SC_ROOM_LIST);
	roomListPacket->setRoomList(needRoomList);
	roomListPacket->setAllRoomCount(roomList.size());
	sendMessage(roomListPacket, mClient);
}
