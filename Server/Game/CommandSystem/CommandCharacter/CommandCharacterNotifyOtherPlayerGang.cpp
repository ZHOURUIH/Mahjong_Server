#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerGang::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerGang* otherGang = NEW_PACKET(otherGang, PT_SC_OTHER_PLAYER_GANG);
	otherGang->mOtherPlayerGUID = mOtherPlayer->getGUID();
	otherGang->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	otherGang->mMahjong = mMahjong;
	sendMessage(otherGang, player);
}