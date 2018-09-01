#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterGetMahjong::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	player->getMahjong(mMahjong);
	// 是真实玩家,则发送消息通知客户端
	if (player->getType() == CT_PLAYER)
	{
		SCNotifyGetMahjong* getStartMahjong = NEW_PACKET(getStartMahjong, PT_SC_NOTIFY_GET_MAHJONG);
		getStartMahjong->mPlayerGUID = player->getGUID();
		getStartMahjong->mMahjong = mMahjong;
		sendMessage(getStartMahjong, player);
	}
}

std::string CommandCharacterGetMahjong::showDebugInfo()
{
	COMMAND_DEBUG("mahjong : %s", MAHJONG_NAME[(int)mMahjong].c_str());
}