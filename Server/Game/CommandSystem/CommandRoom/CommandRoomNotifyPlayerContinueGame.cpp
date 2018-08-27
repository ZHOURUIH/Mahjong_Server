#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomNotifyPlayerContinueGame::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 通知其他已经选择继续游戏的玩家有玩家继续游戏
	auto& playerChooseList = room->getPlayerChooseList();
	auto iter = playerChooseList.begin();
	auto iterEnd = playerChooseList.end();
	FOR(playerChooseList, ; iter != iterEnd; ++iter)
	{
		if (iter->second)
		{
			// 通知其他玩家
			CommandCharacterNotifyOtherPlayerContinueGame* cmdOtherContinue = NEW_CMD_INFO(cmdOtherContinue);
			cmdOtherContinue->mOtherPlayer = mPlayer;
			cmdOtherContinue->mContinue = mContinue;
			mCommandSystem->pushCommand(cmdOtherContinue, iter->first);
			// 通知刚选择继续游戏的玩家其他玩家的信息,只有选择了继续游戏,才会发送其他玩家的选择
			if (mContinue)
			{
				CommandCharacterNotifyOtherPlayerContinueGame* cmdNotifyPlayer = NEW_CMD_INFO(cmdNotifyPlayer);
				cmdNotifyPlayer->mOtherPlayer = iter->first;
				cmdNotifyPlayer->mContinue = mContinue;
				mCommandSystem->pushCommand(cmdNotifyPlayer, mPlayer);
			}
		}
	}
	END(playerChooseList);
	// 不继续则玩家离开房间
	if (!mContinue)
	{
		CommandRoomNotifyPlayerLeave* cmdLeave = NEW_CMD_INFO(cmdLeave);
		cmdLeave->mPlayer = mPlayer;
		cmdLeave->mNotifyOtherPlayer = false;
		mCommandSystem->pushCommand(cmdLeave, room);
	}
	// 玩家选择是否继续游戏
	room->chooseContinueGame(mPlayer, mContinue);
}

std::string CommandRoomNotifyPlayerContinueGame::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}
