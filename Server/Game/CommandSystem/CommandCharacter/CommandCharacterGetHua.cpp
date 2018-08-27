#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterGetHua::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->getHua(mMahjong);

	SCGetHua* hua = NetServer::createPacket(hua, PT_SC_GET_HUA);
	hua->mMahjong = mMahjong;
	mNetServer->sendMessage(hua, player);
}

std::string CommandCharacterGetHua::showDebugInfo()
{
	COMMAND_DEBUG("mahjong : %s", MAHJONG_NAME[(int)mMahjong].c_str());
}