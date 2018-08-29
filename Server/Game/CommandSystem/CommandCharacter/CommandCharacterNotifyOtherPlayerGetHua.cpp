#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerGetHua::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerGetHua* otherGang = NEW_PACKET(otherGang, PT_SC_OTHER_PLAYER_GET_HUA);
	otherGang->mOtherPlayerGUID = mOtherPlayer->getGUID();
	otherGang->mMahjong = mMahjong;
	sendMessage(otherGang, player);
}