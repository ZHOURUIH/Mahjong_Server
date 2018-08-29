#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerAskDrop::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerAskDrop* otherAskDrop = NEW_PACKET(otherAskDrop, PT_SC_OTHER_PLAYER_ASK_DROP);
	otherAskDrop->mOtherPlayerGUID = mOtherPlayer->getGUID();
	sendMessage(otherAskDrop, player);
}