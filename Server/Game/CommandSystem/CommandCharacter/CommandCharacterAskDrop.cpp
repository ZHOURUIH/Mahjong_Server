#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterAskDrop::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 通知玩家打出一张牌
	player->notifyAskDrop();
	// 如果是真实玩家,则需要发送消息到客户端
	if (player->getType() == CT_PLAYER)
	{
		SCAskDrop* getStartDone = NetServer::createPacket(getStartDone, PT_SC_ASK_DROP);
		mNetServer->sendMessage(getStartDone, player);
	}
}