#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterGetStartMahjong::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->getMahjong(mMahjong);
	SCNotifyGetStartMahjong* getStartMahjong = static_cast<SCNotifyGetStartMahjong*>(mNetServer->createPacket(PT_SC_NOTIFY_GET_START_MAHJONG));
	getStartMahjong->mPlayerGUID = player->getGUID();
	getStartMahjong->mMahjong = mMahjong;
	mNetServer->sendMessage(getStartMahjong, player->getClientGUID());
}

std::string CommandCharacterGetStartMahjong::showDebugInfo()
{
	COMMAND_DEBUG("mahjong : %s", MAHJONG_NAME[(int)mMahjong].c_str());
}