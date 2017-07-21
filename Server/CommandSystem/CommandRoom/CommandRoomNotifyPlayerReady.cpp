#include "CommandHeader.h"
#include "Room.h"
#include "CharacterPlayer.h"

void CommandRoomNotifyPlayerReady::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 先通知房间有玩家准备好
	room->notifyPlayerReady(mPlayerGUID, mReady);
	// 通知房间中的其他玩家有玩家已经准备
	txMap<CHAR_GUID, CharacterPlayer*>& playerList = room->getPlayerList();
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = playerList.begin();
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayerEnd = playerList.end();
	FOR_STL(playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->first != mPlayerGUID)
		{
			CommandCharacterNotifyOtherPlayerReady cmd(CMD_PARAM);
			cmd.mPlayerGUID = mPlayerGUID;
			cmd.mReady = mReady;
			mCommandSystem->pushCommand(&cmd, iterPlayer->second);
		}
	}
	END_FOR_STL(playerList);
	// 所有玩家都准备完毕后,通知房间开始游戏
	if (room->isAllPlayerReady())
	{
		CommandRoomStartGame cmdStartGame(CMD_PARAM);
		mCommandSystem->pushCommand(&cmdStartGame, room);
	}
}

std::string CommandRoomNotifyPlayerReady::showDebugInfo()
{
	COMMAND_DEBUG("ready : %s, player : %d", mReady ? "true" : "false", (int)mPlayerGUID);
}
