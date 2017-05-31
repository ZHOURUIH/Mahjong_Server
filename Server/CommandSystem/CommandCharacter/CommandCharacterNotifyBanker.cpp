#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "CharacterManager.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"

void CommandCharacterNotifyBanker::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCNotifyBanker* notifyBanker = static_cast<SCNotifyBanker*>(mNetManagerServer->createPacket(PT_SC_NOTIFY_BANKER));
	notifyBanker->mGUID = mBankerID;
	mNetManagerServer->sendMessage(notifyBanker, player->getClientGUID());
}

std::string CommandCharacterNotifyBanker::showDebugInfo()
{
	COMMAND_DEBUG("banker id : %d", mBankerID);
}