#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerAskDrop::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerAskDrop* otherAskDrop = static_cast<SCOtherPlayerAskDrop*>(mNetServer->createPacket(PT_SC_OTHER_PLAYER_ASK_DROP));
	otherAskDrop->mOtherPlayerGUID = mOtherPlayer->getGUID();
	mNetServer->sendMessage(otherAskDrop, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerAskDrop::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}