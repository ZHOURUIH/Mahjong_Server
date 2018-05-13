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
		setResult(JRR_FULL);
		return;
	}
	if (room->getMember(mPlayerGUID) != NULL)
	{
		setResult(JRR_PLAYER_IN_ROOM);
		return;
	}
	if (room->isRoomLocked())
	{
		setResult(JRR_ROOM_LOCKED);
		return;
	}
	// 灏嗙帺瀹跺姞鍏ユ埧闂翠腑
	CharacterPlayer* joinPlayer = static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(mPlayerGUID));
	room->joinRoom(joinPlayer);
	// 閫氱煡鎴块棿涓殑鍏朵粬鐜╁鏈夌帺瀹跺姞鍏?
	txMap<CHAR_GUID, CharacterPlayer*>& playerList = room->getPlayerList();
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayer = playerList.begin();
	txMap<CHAR_GUID, CharacterPlayer*>::iterator iterPlayerEnd = playerList.end();
	FOR_STL(playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
	{
		if (iterPlayer->second != joinPlayer)
		{
			CommandCharacterNotifyOtherPlayerJoinRoom* cmdJoin = NEW_CMD(cmdJoin);
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
