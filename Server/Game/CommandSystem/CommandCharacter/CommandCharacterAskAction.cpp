#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"
#include "MahjongAction.h"

void CommandCharacterAskAction::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCAskAction* askAction = NetServer::createPacket(askAction, PT_SC_ASK_ACTION);
	askAction->setActionList(mActionList);
	mNetServer->sendMessage(askAction, player);
}