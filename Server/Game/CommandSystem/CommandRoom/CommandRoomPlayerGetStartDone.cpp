#include "CommandHeader.h"
#include "Room.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomPlayerGetStartDone::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	CommandCharacterGetStartDone* cmd = NEW_CMD_INFO(cmd);
	pushCommand(cmd, mCharacterManager->getCharacter(mPlayerGUID));
	// 所有玩家都拿完开局的牌后,通知所有玩家
	if (room->isFull() && room->isAllPlayerGetStartDone())
	{
		room->allPlayerGetStartDone();
	}
}

std::string CommandRoomPlayerGetStartDone::showDebugInfo()
{
	COMMAND_DEBUG("player : %d", (int)mPlayerGUID);
}
