#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerGetStartMahjong::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCNotifyGetStartMahjong* getStartMahjong = static_cast<SCNotifyGetStartMahjong*>(mNetManagerServer->createPacket(PT_SC_NOTIFY_GET_START_MAHJONG));
	getStartMahjong->mPlayerGUID = mPlayerGUID;
	getStartMahjong->mMahjong = mMahjong;
	mNetManagerServer->sendMessage(getStartMahjong, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerGetStartMahjong::showDebugInfo()
{
	COMMAND_DEBUG("player guid : %d, mahjong : %s", mPlayerGUID, MAHJONG_NAME[mMahjong].c_str());
}