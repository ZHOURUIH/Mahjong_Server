#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerReorderMahjong::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCNotifyReorderMahjong* reorderMahjong = NEW_PACKET(reorderMahjong, PT_SC_NOTIFY_REORDER_MAHJONG);
	reorderMahjong->mPlayerGUID = mPlayerGUID;
	sendMessage(reorderMahjong, player);
}

std::string CommandCharacterNotifyOtherPlayerReorderMahjong::showDebugInfo()
{
	COMMAND_DEBUG("player guid : %d", (int)mPlayerGUID);
}