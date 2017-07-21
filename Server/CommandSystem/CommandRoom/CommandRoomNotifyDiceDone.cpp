#include "CommandHeader.h"
#include "Room.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "CharacterManager.h"
#include "CharacterPlayer.h"

void CommandRoomNotifyDiceDone::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 通知房间玩家已经掷完骰子了
	room->notifyDiceDone(mPlayerGUID);
	// 如果全部掷掷完骰子了,则通知房间中的所有玩家开始拿牌
	if (room->isAllPlayerDiceDone())
	{
		room->notifyAllPlayerDiceDone();
		room->setMahjongState(MPS_GET_START);
	}
}

std::string CommandRoomNotifyDiceDone::showDebugInfo()
{
	COMMAND_DEBUG("player guid : %d", mPlayerGUID);
}