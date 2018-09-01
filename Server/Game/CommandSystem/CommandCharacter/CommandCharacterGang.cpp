#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterGang::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->gangMahjong(mMahjong, mDroppedPlayer);
	if (player->getType() == CT_PLAYER)
	{
		SCPlayerGang* gang = NEW_PACKET(gang, PT_SC_PLAYER_GANG);
		gang->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
		gang->mMahjong = mMahjong;
		sendMessage(gang, player);
	}
}