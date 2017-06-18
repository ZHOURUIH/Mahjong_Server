#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyOtherPlayerGetMahjong::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCNotifyGetMahjong* getStartMahjong = static_cast<SCNotifyGetMahjong*>(mNetManagerServer->createPacket(PT_SC_NOTIFY_GET_MAHJONG));
	getStartMahjong->mPlayerGUID = mPlayerGUID;
	getStartMahjong->mMahjong = mMahjong;
	mNetManagerServer->sendMessage(getStartMahjong, player->getClientGUID());
}

std::string CommandCharacterNotifyOtherPlayerGetMahjong::showDebugInfo()
{
	COMMAND_DEBUG("player guid : %d, mahjong : %s", mPlayerGUID, MAHJONG_NAME[mMahjong].c_str());
}