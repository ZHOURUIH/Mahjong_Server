#include "CommandHeader.h"
#include "CharacterManager.h"
#include "MySQLDataBase.h"

void CommandCharacterManagerDestroyCharacter::execute()
{
	CharacterManager* characterManager = static_cast<CharacterManager*>(mReceiver);
	characterManager->destroyCharacter(mGUID);
	mMySQLDataBase->notifyAccountLogin(mGUID, false);
}

std::string CommandCharacterManagerDestroyCharacter::showDebugInfo()
{
	COMMAND_DEBUG("guid : %d", (int)mGUID);
}
