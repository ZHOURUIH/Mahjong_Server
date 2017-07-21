#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterNotifyDiceDone::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCDiceDoneRet* diceDone = static_cast<SCDiceDoneRet*>(mNetServer->createPacket(PT_SC_DICE_DONE_RET));
	mNetServer->sendMessage(diceDone, player->getClientGUID());
}

std::string CommandCharacterNotifyDiceDone::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}