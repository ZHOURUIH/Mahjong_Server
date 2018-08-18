#include "CommandHeader.h"
#include "Room.h"
#include "CharacterPlayer.h"

void CommandRoomNotifyPlayerReady::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 通知房间中的其他玩家有玩家已经准备
	auto& playerList = room->getPlayerList();
	auto iterPlayer = playerList.begin();
	auto iterPlayerEnd = playerList.end();
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
	// 所有玩家都准备完毕后,通知房间开始游戏
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
