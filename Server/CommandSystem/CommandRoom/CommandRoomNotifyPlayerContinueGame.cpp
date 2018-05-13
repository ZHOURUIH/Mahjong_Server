#include "CommandHeader.h"
#include "Room.h"
#include "RoomManager.h"
#include "CharacterPlayer.h"
#include "CharacterManager.h"

void CommandRoomNotifyPlayerContinueGame::execute()
{
	Room* room = static_cast<Room*>(mReceiver);
	// 閫氱煡鍏朵粬宸茬粡閫夋嫨缁х画娓告垙鐨勭帺瀹舵湁鐜╁缁х画娓告垙
	txMap<CharacterPlayer*, bool>& playerChooseList = room->getPlayerChooseList();
	txMap<CharacterPlayer*, bool>::iterator iter = playerChooseList.begin();
	txMap<CharacterPlayer*, bool>::iterator iterEnd = playerChooseList.end();
	FOR_STL(playerChooseList, ; iter != iterEnd; ++iter)
	{
		if (iter->second)
		{
			// 閫氱煡鍏朵粬鐜╁
			CommandCharacterNotifyOtherPlayerContinueGame* cmdOtherContinue = NEW_CMD(cmdOtherContinue);
			cmdOtherContinue->mOtherPlayer = mPlayer;
			cmdOtherContinue->mContinue = mContinue;
			mCommandSystem->pushCommand(cmdOtherContinue, iter->first);
			// 閫氱煡鍒氶€夋嫨缁х画娓告垙鐨勭帺瀹跺叾浠栫帺瀹剁殑淇℃伅,鍙湁閫夋嫨浜嗙户缁父鎴?鎵嶄細鍙戦€佸叾浠栫帺瀹剁殑閫夋嫨
			if (mContinue)
			{
				CommandCharacterNotifyOtherPlayerContinueGame* cmdNotifyPlayer = NEW_CMD(cmdNotifyPlayer);
				cmdNotifyPlayer->mOtherPlayer = iter->first;
				cmdNotifyPlayer->mContinue = mContinue;
				mCommandSystem->pushCommand(cmdNotifyPlayer, mPlayer);
			}
		}
	}
	END_FOR_STL(playerChooseList);
	// 涓嶇户缁垯鐜╁绂诲紑鎴块棿
	if (!mContinue)
	{
		CommandRoomNotifyPlayerLeave* cmdLeave = NEW_CMD(cmdLeave);
		cmdLeave->mPlayer = mPlayer;
		cmdLeave->mNotifyOtherPlayer = false;
		mCommandSystem->pushCommand(cmdLeave, room);
	}
	// 鐜╁閫夋嫨鏄惁缁х画娓告垙
	room->chooseContinueGame(mPlayer, mContinue);
}

std::string CommandRoomNotifyPlayerContinueGame::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}
