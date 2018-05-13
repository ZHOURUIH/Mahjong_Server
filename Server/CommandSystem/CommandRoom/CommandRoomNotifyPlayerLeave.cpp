#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomNotifyPlayerLeave::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 灏嗙帺瀹朵粠鎴块棿涓Щ闄?
	if (room != NULL)
	{
		room->leaveRoom(mPlayer);
	}
	// 鎴块棿涓繕鏈変汉鍒欓€氱煡鎴块棿涓殑鍏朵粬鐜╁鏈夌帺瀹剁寮€
	if (room->getPlayerList().size() > 0)
	{
		// 濡傛灉闇€瑕侀€氱煡鍏朵粬鐜╁
		if (mNotifyOtherPlayer)
		{
			txMap<CHAR_GUID, CharacterPlayer*>& playerList = room->getPlayerList();
			txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = playerList.begin();
			txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayerEnd = playerList.end();
			FOR_STL(playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
			{
				CommandCharacterNotifyOtherPlayerLeaveRoom* cmdLeave = NEW_CMD(cmdLeave);
				cmdLeave->mLeavePlayerID = mPlayer->getGUID();
				mCommandSystem->pushCommand(cmdLeave, iterPlayer->second);
			}
			END_FOR_STL(playerList);
		}
	}
	// 濡傛灉鎴块棿涓病鏈変汉浜?鍒欓攢姣佹埧闂?
	else
	{
		CommandRoomManagerDestroyRoom* cmdDestroyRoom = NEW_CMD(cmdDestroyRoom);
		cmdDestroyRoom->mRoomID = room->getID();
		mCommandSystem->pushCommand(cmdDestroyRoom, mRoomManager);
	}
}

std::string CommandRoomNotifyPlayerLeave::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}
