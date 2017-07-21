#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomNotifyPlayerJoin::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	if (room->isRoomFull())
	{
		mResult = JRR_FULL;
		return;
	}
	if (room->getMember(mPlayerGUID) != NULL)
	{
		mResult = JRR_PLAYER_IN_ROOM;
		return;
	}
	if (room->isRoomLocked())
	{
		mResult = JRR_ROOM_LOCKED;
		return;
	}
	// 将玩家加入房间中
	CharacterPlayer* joinPlayer = static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(mPlayerGUID));
	room->joinRoom(joinPlayer);
	// 通知房间中的其他玩家有玩家加入
	txMap<CHAR_GUID, CharacterPlayer*>& playerList = room->getPlayerList();
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = playerList.begin();
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayerEnd = playerList.end();
	FOR_STL(playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != joinPlayer)
		{
			CommandCharacterNotifyOtherPlayerJoinRoom cmdJoin(CMD_PARAM);
			cmdJoin.mJoinPlayer = joinPlayer;
			mCommandSystem->pushCommand(&cmdJoin, iterPlayer->second);
		}
	}
	END_FOR_STL(playerList);
	mResult = JRR_SUCC;
}

std::string CommandRoomNotifyPlayerJoin::showDebugInfo()
{
	COMMAND_DEBUG("player id : %d", (int)mPlayerGUID);
}
