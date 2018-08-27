#include "CommandHeader.h"
#include "Room.h"
#include "CharacterPlayer.h"

void CommandRoomNotifyPlayerGetStartDone::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 所有玩家都拿完开局的牌后,通知所有玩家
	if (room->isFull() && room->isAllPlayerGetStartDone())
	{
		room->notifyAllPlayerGetStartDone();
		room->setMahjongState(MPS_NORMAL_GAMING);
		// 通知庄家打出一张牌
		room->playerAskDrop(room->getBanker());
	}
}

std::string CommandRoomNotifyPlayerGetStartDone::showDebugInfo()
{
	COMMAND_DEBUG("player : %d", (int)mPlayerGUID);
}
