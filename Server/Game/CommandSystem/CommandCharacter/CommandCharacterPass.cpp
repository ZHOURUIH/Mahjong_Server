#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterPass::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCPlayerPass* pass = NEW_PACKET(pass, PT_SC_PLAYER_PASS);
	pass->mDroppedPlayerGUID = mDroppedPlayer->getGUID();
	pass->mMahjong = mMahjong;
	sendMessage(pass, player);
}

std::string CommandCharacterPass::showDebugInfo()
{
	COMMAND_DEBUG("mahjong : %s", MAHJONG_NAME[(int)mMahjong].c_str());
}