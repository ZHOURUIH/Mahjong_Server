#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyAllGetStartDone::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCNotifyAllGetStartDone* getStartDone = NetServer::createPacket(getStartDone, PT_SC_NOTIFY_ALL_GET_START_DONE);
	mNetServer->sendMessage(getStartDone, player);
}