#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerContinueGame::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	if (mContinue)
	{
		SCOtherPlayerContinueGame* otherContinue = static_cast<SCOtherPlayerContinueGame*>(mNetServer->createPacket(PT_SC_OTHER_PLAYER_CONTINUE_GAME));
		otherContinue->mOtherPlayerGUID = mOtherPlayer->getGUID();
		mNetServer->sendMessage(otherContinue, player->getClientGUID());
	}
	else
	{
		SCOtherPlayerBackToMahjongHall* otherBack = static_cast<SCOtherPlayerBackToMahjongHall*>(mNetServer->createPacket(PT_SC_OTHER_PLAYER_BACK_TO_MAHJONG_HALL));
		otherBack->mOtherPlayerGUID = mOtherPlayer->getGUID();
		mNetServer->sendMessage(otherBack, player->getClientGUID());
	}
}

std::string CommandCharacterNotifyOtherPlayerContinueGame::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}