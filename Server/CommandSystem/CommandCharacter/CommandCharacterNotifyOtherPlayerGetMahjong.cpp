#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerGetMahjong::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCNotifyGetMahjong* getStartMahjong = static_cast<SCNotifyGetMahjong*>(mNetServer->createPacket(PT_SC_NOTIFY_GET_MAHJONG));
	getStartMahjong->mPlayerGUID = mPlayerGUID;
	getStartMahjong->mMahjong = mMahjong;
	mNetServer->sendMessage(getStartMahjong, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerGetMahjong::showDebugInfo()
{
	COMMAND_DEBUG("player guid : %d, mahjong : %s", (int)mPlayerGUID, MAHJONG_NAME[(int)mMahjong].c_str());
}