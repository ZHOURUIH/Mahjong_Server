#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterGetStartMahjong::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->getMahjongStart(mMahjong);
	SCNotifyGetStartMahjong* getStartMahjong = static_cast<SCNotifyGetStartMahjong*>(mNetManagerServer->createPacket(PT_SC_NOTIFY_GET_START_MAHJONG));
	getStartMahjong->mPlayerGUID = player->getGUID();
	getStartMahjong->mMahjong = mMahjong;
	mNetManagerServer->sendMessage(getStartMahjong, player->getClientGUID());
}

std::string CommandCharacterGetStartMahjong::showDebugInfo()
{
	COMMAND_DEBUG("mahjong : %s", MAHJONG_NAME[mMahjong].c_str());
}