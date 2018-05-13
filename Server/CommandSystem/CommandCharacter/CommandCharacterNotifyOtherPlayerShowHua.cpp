#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerShowHua::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerShowHua* otherGang = static_cast<SCOtherPlayerShowHua*>(mNetServer->createPacket(PT_SC_OTHER_PLAYER_SHOW_HUA));
	otherGang->mOtherPlayerGUID = mOtherPlayer->getGUID();
	otherGang->mIndex = mIndex;
	otherGang->mMahjong = mMahjong;
	mNetServer->sendMessage(otherGang, player->getClientGUID());
}