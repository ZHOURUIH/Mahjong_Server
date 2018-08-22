#include "CommandHeader.h"
#include "CharacterManager.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "NetClient.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"

void CommandCharacterManagerRobotLogin::execute()
{
	CommandCharacterManagerCreateCharacter* cmdCreate = NEW_CMD_INFO(cmdCreate);
	cmdCreate->mCharGUID = mGUID;
	cmdCreate->mName = mName;
	cmdCreate->mType = CT_MAHJONG_ROBOT;
	cmdCreate->mClientGUID = INVALID_ID;
	mCommandSystem->pushCommand(cmdCreate, mReceiver);

	// 玩家登陆后,设置玩家属性
	Character* player = mCharacterManager->getCharacter(mGUID);
	CharacterData* data = player->getCharacterData();
	data->mHead = mHead;
	data->mMoney = mMoney;
	data->mBanker = false;
	data->mPosition = -1;
	data->mRoomID = INVALID_INT_ID;
	data->mReady = false;
}

std::string CommandCharacterManagerRobotLogin::showDebugInfo()
{
	COMMAND_DEBUG("guid : %d, name : %s", (int)mGUID, mName.c_str());
}
