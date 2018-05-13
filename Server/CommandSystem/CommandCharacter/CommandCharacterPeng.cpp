#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterPeng::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->pengMahjong(mMahjong);
	SCPlayerPeng* peng = static_cast<SCPlayerPeng*>(mNetServer->createPacket(PT_SC_PLAYER_PENG));
	peng->mMahjong = mMahjong;
	peng->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	mNetServer->sendMessage(peng, player->getClientGUID());
}

std::string CommandCharacterPeng::showDebugInfo()
{
	COMMAND_DEBUG("mahjong : %s", MAHJONG_NAME[(int)mMahjong].c_str());
}