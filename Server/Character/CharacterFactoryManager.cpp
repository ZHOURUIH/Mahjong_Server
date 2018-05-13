#include "Character.h"
#include "CharacterPlayer.h"
#include "CharacterNPC.h"
#include "CharacterFactory.h"
#include "CharacterFactoryManager.h"

void CharacterFactoryManager::init()
{
	addFactory<CharacterFactory<Character> >(CT_NONE); 
	addFactory<CharacterFactory<CharacterNPC> >(CT_NPC);
	addFactory<CharacterFactory<CharacterPlayer> >(CT_PLAYER);

	if (mFactoryList.size() < CT_MAX)
	{
		LOG_ERROR("error : not all character factory registered! registered count : %d, max count : %d", (int)mFactoryList.size(), CT_MAX);
	}
}