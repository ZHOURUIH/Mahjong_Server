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
	SCPlayerPeng* peng = NEW_PACKET(peng, PT_SC_PLAYER_PENG);
	peng->mMahjong = mMahjong;
	peng->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	sendMessage(peng, player);
}

std::string CommandCharacterPeng::showDebugInfo()
{
	COMMAND_DEBUG("mahjong : %s", MAHJONG_NAME[(int)mMahjong].c_str());
}