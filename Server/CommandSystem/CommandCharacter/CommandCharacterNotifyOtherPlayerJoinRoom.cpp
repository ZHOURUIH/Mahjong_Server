#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerJoinRoom::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 发送消息通知客户端
	CharacterData* joinCharacterData = mJoinPlayer->getCharacterData();
	SCOtherPlayerJoinRoom* joinRoom = static_cast<SCOtherPlayerJoinRoom*>(mNetServer->createPacket(PT_SC_OTHER_PLAYER_JOIN_ROOM));
	joinRoom->mPlayerGUID = mJoinPlayer->getGUID();
	joinRoom->setName(mJoinPlayer->getName());
	joinRoom->mMoney = joinCharacterData->mMoney;
	joinRoom->mHead = joinCharacterData->mHead;
	joinRoom->mPosition = joinCharacterData->mPosition;
	joinRoom->mReady = joinCharacterData->mReady;
	joinRoom->mBanker = joinCharacterData->mBanker;
	mNetServer->sendMessage(joinRoom, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerJoinRoom::showDebugInfo()
{
	COMMAND_DEBUG("player guid : %d, name : %s", mJoinPlayer->getGUID(), mJoinPlayer->getName().c_str());
}