#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerPeng::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerPeng* otherPeng = static_cast<SCOtherPlayerPeng*>(mNetServer->createPacket(PT_SC_OTHER_PLAYER_PENG));
	otherPeng->mOtherPlayerGUID = mOtherPlayer->getGUID();
	otherPeng->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	otherPeng->mMahjong = mMahjong;
	mNetServer->sendMessage(otherPeng, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerPeng::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}