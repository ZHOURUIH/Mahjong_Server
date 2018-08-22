#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomNotifyPlayerJoin::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	if (room->isFull())
	{
		setResult(JRR_FULL);
		return;
	}
	if (room->getMember(mPlayerGUID) != NULL)
	{
		setResult(JRR_PLAYER_IN_ROOM);
		return;
	}
	// 将玩家加入房间中
	CharacterPlayer* joinPlayer = static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(mPlayerGUID));
	room->joinRoom(joinPlayer);
	// 通知房间中的其他玩家有玩家加入
	auto& playerList = room->getPlayerList();
	auto iterPlayer = playerList.begin();
	auto iterPlayerEnd = playerList.end();
	FOR_STL(playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != joinPlayer)
		{
			CommandCharacterNotifyOtherPlayerJoinRoom* cmdJoin = NEW_CMD_INFO(cmdJoin);
			cmdJoin->mJoinPlayer = joinPlayer;
			mCommandSystem->pushCommand(cmdJoin, iterPlayer->second);
		}
	}
	END_FOR_STL(playerList);
	setResult(JRR_SUCCESS);
}

std::string CommandRoomNotifyPlayerJoin::showDebugInfo()
{
	COMMAND_DEBUG("player id : %d", (int)mPlayerGUID);
}
