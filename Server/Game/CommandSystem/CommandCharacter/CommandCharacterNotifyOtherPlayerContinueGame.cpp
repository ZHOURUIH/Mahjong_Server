#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerContinueGame::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerContinueGame* otherContinue = NEW_PACKET(otherContinue, PT_SC_OTHER_PLAYER_CONTINUE_GAME);
	otherContinue->mOtherPlayerGUID = mOtherPlayer->getGUID();
	otherContinue->mBanker = mOtherPlayer->getCharacterData()->mBanker;
	sendMessage(otherContinue, player);
}