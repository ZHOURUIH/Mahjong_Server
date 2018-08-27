#include "CharacterMahjongRobot.h"
#include "CommandHeader.h"
#include "CharacterData.h"
#include "MahjongAction.h"
#include "RoomManager.h"
#include "Room.h"

void CharacterMahjongRobot::notifyStartGame()
{
	CommandCharacterGetStartDone* cmdStartDone = NEW_CMD_INFO(cmdStartDone);
	mCommandSystem->pushCommand(cmdStartDone, this);
}

void CharacterMahjongRobot::notifyAskAction(txVector<MahjongAction*>& actionList)
{
	// 默认选择第一个操作
	if (actionList.size() > 0)
	{
		CommandRoomConfirmAction* cmd = NEW_CMD_INFO(cmd);
		cmd->mPlayerGUID = mCharacterData->mGUID;
		cmd->mAction = actionList[0]->mType;
		mCommandSystem->pushCommand(cmd, mRoomManager->getRoom(mCharacterData->mRoomID));
	}
}

void CharacterMahjongRobot::notifyAskDrop()
{
	// 打出最后一张牌
	CommandRoomRequestDrop* cmd = NEW_CMD_INFO(cmd);
	cmd->mPlayerGUID = mCharacterData->mGUID;
	cmd->mIndex = mCharacterData->mHandIn.size() - 1;
	mCommandSystem->pushCommand(cmd, mRoomManager->getRoom(mCharacterData->mRoomID));
}