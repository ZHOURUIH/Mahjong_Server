#include "CommandHeader.h"
#include "CharacterManager.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "NetManagerClient.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"

void CommandCharacterManagerPlayerLogin::execute()
{
	CommandCharacterManagerCreateCharacter cmdCreate(COMMAND_PARAM);
	cmdCreate.mCharGUID = mGUID;
	cmdCreate.mName = mName;
	cmdCreate.mType = CT_PLAYER;
	cmdCreate.mClientGUID = mClient;
	mCommandSystem->pushCommand(&cmdCreate, mReceiver);

	// 玩家登陆后,设置玩家属性
	Character* player = cmdCreate.mResultCharacter;
	CharacterData* data = player->getCharacterData();
	data->mHead = mHead;
	data->mMoney = mMoney;
	data->mBanker = false;
	data->mPosition = -1;
	data->mRoomID = INVALID_ID;
	data->mReady = false;

	// 玩家登陆成功后, 发回登陆成功的消息
	SCLoginRet* loginRet = static_cast<SCLoginRet*>(mNetManagerServer->createPacket(PT_SC_LOGIN_RET));
	loginRet->mLoginRet = 1;
	loginRet->mGUID = mGUID;
	loginRet->mMoney = mMoney;
	loginRet->mHead = mHead;
	loginRet->setName(mName);
	mNetManagerServer->sendMessage(loginRet, mClient);

	NetManagerClient* client = mNetManagerServer->getClient(mClient);
	if (client != NULL)
	{
		client->notifyPlayerLogin(mGUID);
	}
}

std::string CommandCharacterManagerPlayerLogin::showDebugInfo()
{
	COMMAND_DEBUG("guid : %d, name : %s", (int)mGUID, mName.c_str());
}
