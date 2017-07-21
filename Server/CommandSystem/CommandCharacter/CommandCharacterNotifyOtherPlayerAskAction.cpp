#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerAskAction::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerAskAction* otherAskAction = static_cast<SCOtherPlayerAskAction*>(mNetServer->createPacket(PT_SC_OTHER_PLAYER_ASK_ACTION));
	otherAskAction->mOtherPlayerGUID = mOtherPlayer->getGUID();
	mNetServer->sendMessage(otherAskAction, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerAskAction::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}