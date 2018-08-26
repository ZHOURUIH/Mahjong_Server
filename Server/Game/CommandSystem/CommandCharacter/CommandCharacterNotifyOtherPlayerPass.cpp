#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerPass::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerPass* otherPass = NetServer::createPacket(otherPass, PT_SC_OTHER_PLAYER_PASS);
	otherPass->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	otherPass->mOtherPlayerGUID = mOtherPlayer->getGUID();
	otherPass->mMahjong = mMahjong;
	mNetServer->sendMessage(otherPass, player);
}