#include "CommandHeader.h"
#include "CharacterManager.h"

void CommandCharacterManagerCreateCharacter::execute()
{
	CharacterManager* characterManager = static_cast<CharacterManager*>(mReceiver);
	characterManager->createCharacter(mName, mType, mCharGUID, mClientGUID);
}

std::string CommandCharacterManagerCreateCharacter::showDebugInfo()
{
	COMMAND_DEBUG("guid : %d, client : %d, name : %s, type : %d", (int)mCharGUID, (int)mClientGUID, mName.c_str(), mType);
}