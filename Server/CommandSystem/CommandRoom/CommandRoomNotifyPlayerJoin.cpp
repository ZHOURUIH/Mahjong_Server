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
	room->joinRoom(static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(mPlayerGUID)));
	// 通知房间中的其他玩家有玩家加入
	std::map<CHAR_GUID, CharacterPlayer*>::const_iterator iterPlayer = room->getPlayerList().begin();
	std::map<CHAR_GUID, CharacterPlayer*>::const_iterator iterPlayerEnd = room->getPlayerList().end();
	for (; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second->getGUID() != mPlayerGUID)
		{
			CommandCharacterNotifyOtherPlayerJoinRoom cmdJoin(COMMAND_PARAM);
			cmdJoin.mJoinPlayerID = mPlayerGUID;
			mCommandSystem->pushCommand(&cmdJoin, iterPlayer->second);
		}
	}
	mResult = JRR_SUCC;
}

std::string CommandRoomNotifyPlayerJoin::showDebugInfo()
{
	COMMAND_DEBUG("player id : %d", (int)mPlayerGUID);
}
