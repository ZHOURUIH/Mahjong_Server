#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterAskAction::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	// 通知玩家做出选择行为
	player->notifyAskAction(mActionList);
	// 如果是真实玩家,则需要发送消息到客户端
	if (player->getType() == CT_PLAYER)
	{
		SCAskAction* askAction = NEW_PACKET(askAction, PT_SC_ASK_ACTION);
		askAction->setActionList(mActionList);
		sendMessage(askAction, player);
	}
}