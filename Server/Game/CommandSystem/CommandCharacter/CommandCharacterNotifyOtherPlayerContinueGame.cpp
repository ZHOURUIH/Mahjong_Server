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
		SCOtherPlayerContinueGame* otherContinue = NetServer::createPacket(otherContinue, PT_SC_OTHER_PLAYER_CONTINUE_GAME);
		otherContinue->mOtherPlayerGUID = mOtherPlayer->getGUID();
		otherContinue->mBanker = mOtherPlayer->getCharacterData()->mBanker;
		mNetServer->sendMessage(otherContinue, player);
	}
	else
	{
		SCOtherPlayerBackToMahjongHall* otherBack = NetServer::createPacket(otherBack, PT_SC_OTHER_PLAYER_BACK_TO_MAHJONG_HALL);
		otherBack->mOtherPlayerGUID = mOtherPlayer->getGUID();
		mNetServer->sendMessage(otherBack, player);
	}
}