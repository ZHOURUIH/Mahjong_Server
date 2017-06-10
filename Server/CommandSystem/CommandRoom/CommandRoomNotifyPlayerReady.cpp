#include "CommandHeader.h"
#include "Room.h"
#include "CharacterPlayer.h"

void CommandRoomNotifyPlayerReady::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 先通知房间有玩家准备好
	room->notifyPlayerReady(mPlayerGUID, mReady);
	// 通知房间中的其他玩家有玩家已经准备
	std::map<CHAR_GUID, CharacterPlayer*>::const_iterator iterPlayer = room->getPlayerList().begin();
	std::map<CHAR_GUID, CharacterPlayer*>::const_iterator iterPlayerEnd = room->getPlayerList().end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->first != mPlayerGUID)
		{
			CommandCharacterNotifyOtherPlayerReady cmd(CMD_PARAM);
			cmd.mPlayerGUID = mPlayerGUID;
			cmd.mReady = mReady;
			mCommandSystem->pushCommand(&cmd, iterPlayer->second);
		}
	}
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
