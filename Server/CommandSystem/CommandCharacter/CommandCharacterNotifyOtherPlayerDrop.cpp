#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerDrop::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCOtherPlayerDrop* otherPlayerDrop = static_cast<SCOtherPlayerDrop*>(mNetServer->createPacket(PT_SC_OTHER_PLAYER_DROP));
	otherPlayerDrop->mPlayerGUID = mPlayerGUID;
	otherPlayerDrop->mMahjong = mMahjong;
	otherPlayerDrop->mIndex = mIndex;
	mNetServer->sendMessage(otherPlayerDrop, player->getClientGUID());

	// 打出一张牌后需要重新排列
	CommandCharacterNotifyOtherPlayerReorderMahjong* cmdReorder = NEW_CMD(cmdReorder);
	cmdReorder->mPlayerGUID = mPlayerGUID;
	mCommandSystem->pushCommand(cmdReorder, player);
}

std::string CommandCharacterNotifyOtherPlayerDrop::showDebugInfo()
{
	COMMAND_DEBUG("player guid : %d, mahjong : %s", mPlayerGUID, MAHJONG_NAME[mMahjong].c_str());
}