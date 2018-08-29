#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterReorderMahjong::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->reorderMahjong();
	SCNotifyReorderMahjong* reorder = NEW_PACKET(reorder, PT_SC_NOTIFY_REORDER_MAHJONG);
	reorder->mPlayerGUID = player->getGUID();
	sendMessage(reorder, player);
}

std::string CommandCharacterReorderMahjong::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}