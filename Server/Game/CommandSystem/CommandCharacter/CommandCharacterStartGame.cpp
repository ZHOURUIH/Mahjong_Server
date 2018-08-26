#include "CommandHeader.h"
#include "CharacterPlayer.h"
#include "PacketHeader.h"
#include "NetServer.h"
#include "CharacterData.h"

void CommandCharacterStartGame::execute()
{
	CharacterPlayer* player = static_cast<CharacterPlayer*>(mReceiver);
	SCStartGame* startGamePacket = NetServer::createPacket(startGamePacket, PT_SC_START_GAME);
	startGamePacket->setDice(mDice[0], mDice[1]);
	startGamePacket->setMahjongList(mPlayerIDList, mHandInList, mHuaList);
	mNetServer->sendMessage(startGamePacket, player);
}

std::string CommandCharacterStartGame::showDebugInfo()
{
	COMMAND_DEBUG("dice : %d, %d", (int)mDice[0], (int)mDice[1]);
}