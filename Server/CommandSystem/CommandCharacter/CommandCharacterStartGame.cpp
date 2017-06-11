#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetManagerServer.h"
#include "CharacterData.h"

void CommandCharacterStartGame::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCStartGame* startGamePacket = static_cast<SCStartGame*>(mNetManagerServer->createPacket(PT_SC_START_GAME));
	startGamePacket->setDice(mDice[0], mDice[1]);
	mNetManagerServer->sendMessage(startGamePacket, player->getClientGUID());
}

std::string CommandCharacterStartGame::showDebugInfo()
{
	COMMAND_DEBUG("dice : %d, %d", (int)mDice[0], (int)mDice[1]);
}