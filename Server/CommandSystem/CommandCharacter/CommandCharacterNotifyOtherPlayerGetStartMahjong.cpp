#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerGetStartMahjong::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCNotifyGetStartMahjong* getStartMahjong = static_cast<SCNotifyGetStartMahjong*>(mNetServer->createPacket(PT_SC_NOTIFY_GET_START_MAHJONG));
	getStartMahjong->mPlayerGUID = mPlayerGUID;
	getStartMahjong->mMahjong = mMahjong;
	mNetServer->sendMessage(getStartMahjong, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerGetStartMahjong::showDebugInfo()
{
	COMMAND_DEBUG("player guid : %d, mahjong : %s", (int)mPlayerGUID, MAHJONG_NAME[(int)mMahjong].c_str());
}