#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterGetMahjong::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->getMahjongStart(mMahjong);
	SCNotifyGetMahjong* getStartMahjong = static_cast<SCNotifyGetMahjong*>(mNetManagerServer->createPacket(PT_SC_NOTIFY_GET_MAHJONG));
	getStartMahjong->mPlayerGUID = player->getGUID();
	getStartMahjong->mMahjong = mMahjong;
	mNetManagerServer->sendMessage(getStartMahjong, player->getClientGUID());
}

std::string CommandCharacterGetMahjong::showDebugInfo()
{
	COMMAND_DEBUG("mahjong : %s", MAHJONG_NAME[mMahjong].c_str());
}