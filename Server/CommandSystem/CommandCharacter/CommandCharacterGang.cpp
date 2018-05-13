#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterGang::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->gangMahjong(mMahjong, mDroppedPlayer);
	SCPlayerGang* gang = static_cast<SCPlayerGang*>(mNetServer->createPacket(PT_SC_PLAYER_GANG));
	gang->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	gang->mMahjong = mMahjong;
	mNetServer->sendMessage(gang, player->getClientGUID());
}