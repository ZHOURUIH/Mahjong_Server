#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomNotifyPlayerLeave::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 将玩家从房间中移除
	if (room != NULL)
	{
		room->leaveRoom(mPlayer);
	}
	// 房间中还有人则通知房间中的其他玩家有玩家离开
	if (room->getPlayerList().size() > 0)
	{
		// 如果需要通知其他玩家
		if (mNotifyOtherPlayer)
		{
			auto& playerList = room->getPlayerList();
			auto iterPlayer = playerList.begin();
			auto iterPlayerEnd = playerList.end();
			FOR_STL(playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
			{
				CommandCharacterNotifyOtherPlayerLeaveRoom* cmdLeave = NEW_CMD_INFO(cmdLeave);
				cmdLeave->mLeavePlayerID = mPlayer->getGUID();
				mCommandSystem->pushCommand(cmdLeave, iterPlayer->second);
			}
			END_FOR_STL(playerList);
		}
	}
	// 如果房间中没有人了,则销毁房间
	else
	{
		CommandRoomManagerDestroyRoom* cmdDestroyRoom = NEW_CMD_INFO(cmdDestroyRoom);
		cmdDestroyRoom->mRoomID = room->getID();
		mCommandSystem->pushCommand(cmdDestroyRoom, mRoomManager);
	}
}

std::string CommandRoomNotifyPlayerLeave::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}
