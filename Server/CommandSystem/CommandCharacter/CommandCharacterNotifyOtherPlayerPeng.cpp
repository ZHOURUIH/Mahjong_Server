#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerPeng::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerPeng* otherPeng = NetServer::createPacket(otherPeng, PT_SC_OTHER_PLAYER_PENG);
	otherPeng->mOtherPlayerGUID = mOtherPlayer->getGUID();
	otherPeng->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	otherPeng->mMahjong = mMahjong;
	mNetServer->sendMessage(otherPeng, player);
}