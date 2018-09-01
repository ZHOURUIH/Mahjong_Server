#include "CharacterMahjongRobot.h"
#include "CommandHeader.h"
#include "CharacterData.h"
#include "MahjongAction.h"
#include "RoomManager.h"
#include "Room.h"

void CharacterMahjongRobot::update(float elapsedTime)
{
	;
}

void CharacterMahjongRobot::notifyStartGame()
{
	CommandRoomPlayerGetStartDone* cmd = NEW_CMD_INFO(cmd);
	cmd->mPlayerGUID = mCharacterData->mGUID;
	pushCommand(cmd, mRoomManager->getRoom(mCharacterData->mRoomID));
}

void CharacterMahjongRobot::notifyAskAction(txVector<MahjongAction*>& actionList)
{
	if (mAskAcitonCmdID != -1)
	{
		LOG_ERROR("ask action cmd id is NOT -1, can not ask action again!");
		return;
	}
	// 默认选择第一个操作
	if (actionList.size() > 0)
	{
		CommandRoomConfirmAction* cmd = NEW_CMD_DELAY_INFO(cmd);
		cmd->mPlayerGUID = mCharacterData->mGUID;
		cmd->mAction = actionList[0]->mType;
		cmd->addStartCommandCallback(onCmdStart, this);
		pushDelayCommand(cmd, mRoomManager->getRoom(mCharacterData->mRoomID));
		mAskAcitonCmdID = cmd->getAssignID();
	}
}

void CharacterMahjongRobot::notifyAskDrop()
{
	if (mRequestDropCmdID != -1)
	{
		LOG_ERROR("request drop cmd id is NOT -1, can not request drop again!");
		return;
	}
	// 打出最后一张牌
	CommandRoomRequestDrop* cmd = NEW_CMD_DELAY_INFO(cmd);
	cmd->mPlayerGUID = mCharacterData->mGUID;
	cmd->mIndex = mCharacterData->mHandIn.size() - 1;
	cmd->addStartCommandCallback(onCmdStart, this);
	pushDelayCommand(cmd, mRoomManager->getRoom(mCharacterData->mRoomID));
	mRequestDropCmdID = cmd->getAssignID();
}

void CharacterMahjongRobot::notifyLeaveRoom()
{
	clearCmd();
}

void CharacterMahjongRobot::clearCmd()
{
	// 中断所有未执行的命令
	if (mAskAcitonCmdID != -1)
	{
		mCommandSystem->interruptCommand(mAskAcitonCmdID);
		mAskAcitonCmdID = -1;
	}
	if (mRequestDropCmdID != -1)
	{
		mCommandSystem->interruptCommand(mRequestDropCmdID);
		mRequestDropCmdID = -1;
	}
}

void CharacterMahjongRobot::onCmdStart(txCommand* cmd, void* user_data)
{
	CharacterMahjongRobot* robot = (CharacterMahjongRobot*)user_data;
	int assignID = cmd->getAssignID();
	if (assignID == robot->mAskAcitonCmdID)
	{
		robot->mAskAcitonCmdID = -1;
	}
	else if (assignID == robot->mRequestDropCmdID)
	{
		robot->mRequestDropCmdID = -1;
	}
}