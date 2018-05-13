#include "CommandHeader.h"
#include "Room.h"
#include "CharacterPlayer.h"

void CommandRoomNotifyPlayerReady::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 閫氱煡鎴块棿涓殑鍏朵粬鐜╁鏈夌帺瀹跺凡缁忓噯澶?
	txMap<CHAR_GUID, CharacterPlayer*>& playerList = room->getPlayerList();
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = playerList.begin();
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayerEnd = playerList.end();
	FOR_STL(playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->first != mPlayerGUID)
		{
			CommandCharacterNotifyOtherPlayerReady* cmd = NEW_CMD(cmd);
			cmd->mPlayerGUID = mPlayerGUID;
			cmd->mReady = mReady;
			mCommandSystem->pushCommand(cmd, iterPlayer->second);
		}
	}
	END_FOR_STL(playerList);
	// 鎵€鏈夌帺瀹堕兘鍑嗗瀹屾瘯鍚?閫氱煡鎴块棿寮€濮嬫父鎴?
	if (room->isRoomFull() && room->isAllPlayerReady())
	{
		CommandRoomStartGame* cmd = NEW_CMD(cmd);
		 mCommandSystem->pushCommand(cmd, room);
	}
}

std::string CommandRoomNotifyPlayerReady::showDebugInfo()
{
	COMMAND_DEBUG("ready : %s, player : %d", mReady ? "true" : "false", (int)mPlayerGUID);
}
