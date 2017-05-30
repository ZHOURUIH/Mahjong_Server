#include "CommandHeader.h"
#include "CharacterManager.h"

void CommandCharacterManagerDestroyCharacter::execute()
{
	CharacterManager* characterManager = static_cast<CharacterManager*>(mReceiver);
	characterManager->destroyCharacter(mGUID);
}

std::string CommandCharacterManagerDestroyCharacter::showDebugInfo()
{
	COMMAND_DEBUG("guid : %d", (int)mGUID);
}
