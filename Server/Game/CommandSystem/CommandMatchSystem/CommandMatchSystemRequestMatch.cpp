#include "CommandHeader.h"
#include "MatchSystem.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"

void CommandMatchSystemRequestMatch::execute()
{
	MatchSystem* matchSystem = CMD_CAST<MatchSystem*>(mReceiver);
	matchSystem->requestFreeMatch(mPlayer);
}

std::string CommandMatchSystemRequestMatch::showDebugInfo()
{
	COMMAND_DEBUG("player name : %s, id : %d", mPlayer->getName().c_str(), mPlayer->getCharacterData()->mGUID);
}