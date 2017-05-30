#include "txUtility.h"

#include "CharacterManager.h"
#include "CharacterFactory.h"
#include "CharacterData.h"
#include "CharacterPlayer.h"
#include "CharacterFactoryManager.h"

CharacterManager::CharacterManager()
:
txCommandReceiver("CharacterManager"),
mLockUpdate(false)
{
	mCharacterFactoryManager = TRACE_NEW(CharacterFactoryManager, mCharacterFactoryManager);
	mCharacterFactoryManager->init();
}

void CharacterManager::update(const float& elapsedTime)
{
	if (isLocked())
	{
		return;
	}
	std::map<std::string, Character*>::iterator characterBegin = mCharacterList.begin();
	std::map<std::string, Character*>::iterator characterEnd = mCharacterList.end();
	for (; characterBegin != characterEnd; ++characterBegin)
	{
		Character* character = characterBegin->second;
		if (NULL != character)
		{
			character->update(elapsedTime);
		}
	}
}

void CharacterManager::destroy()
{
	std::map<CHARACTER_TYPE, std::map<std::string, Character*> >::iterator iterType = mCharacterTypeList.begin();
	std::map<CHARACTER_TYPE, std::map<std::string, Character*> >::iterator iterTypeEnd = mCharacterTypeList.end();
	for (; iterType != iterTypeEnd; ++iterType)
	{
		CharacterFactoryBase* characterFactory = mCharacterFactoryManager->getFactory(iterType->first);
		std::map<std::string, Character*>::iterator iterChar = iterType->second.begin();
		std::map<std::string, Character*>::iterator iterCharEnd = iterType->second.end();
		for (; iterChar != iterCharEnd; ++iterChar)
		{
			characterFactory->destroyCharacter(iterChar->second);
		}
	}
	mCharacterList.clear();
	mCharacterTypeList.clear();
}

Character* CharacterManager::createCharacter(const std::string& name, const CHARACTER_TYPE& type, const CHAR_GUID& id, const CLIENT_GUID& client)
{
	if (mCharacterList.find(name) != mCharacterList.end())
	{
		GAME_ERROR("error : there is a character named : %s! can not create again!", name.c_str());
		return NULL;
	}
	CharacterFactoryBase* factory = mCharacterFactoryManager->getFactory(type);
	if(NULL == factory)
	{
		GAME_ERROR("error : can not find CharacterFactory ,type %d ", (int)type);
		return NULL;
	}

	Character* newChar = factory->createCharacter(name);
	if (newChar != NULL)
	{
		if (type == CT_PLAYER)
		{
			static_cast<CharacterPlayer*>(newChar)->setClientGUID(client);
		}
		newChar->init(id);
		addCharacterToList(newChar);
	}
	return newChar;
}

void CharacterManager::addCharacterToList(Character* character)
{
	if (character == NULL)
	{
		return;
	}
	// 加入到全部角色列表
	mCharacterList.insert(std::make_pair(character->getName(), character));
	// 加入到角色分类列表
	std::map<CHARACTER_TYPE, std::map<std::string, Character*> >::iterator iterType = mCharacterTypeList.find(character->getType());
	if (iterType != mCharacterTypeList.end())
	{
		iterType->second.insert(std::make_pair(character->getName(), character));
	}
	else
	{
		std::map<std::string, Character*> characterMap;
		characterMap.insert(std::make_pair(character->getName(), character));
		mCharacterTypeList.insert(std::make_pair(character->getType(), characterMap));
	}
	// 加入ID索引表
	int characterID = character->getGUID();
	if (mCharacterIDList.find(characterID) != mCharacterIDList.end())
	{
		GAME_ERROR("error : there is a character id : %d", characterID);
	}
	mCharacterIDList.insert(std::make_pair(characterID, character));
}

void CharacterManager::removeCharacterFromList(Character* character)
{
	if (character == NULL)
	{
		return;
	}
	// 从全部角色列表中移除
	std::map<std::string, Character*>::iterator iter = mCharacterList.find(character->getName());
	if (iter != mCharacterList.end())
	{
		mCharacterList.erase(iter);
	}
	// 从角色分类列表中移除
	std::map<CHARACTER_TYPE, std::map<std::string, Character*> >::iterator iterType = mCharacterTypeList.find(character->getType());
	if (iterType != mCharacterTypeList.end())
	{
		std::map<std::string, Character*>::iterator iterChar = iterType->second.find(character->getName());
		if (iterChar != iterType->second.end())
		{
			iterType->second.erase(iterChar);
		}
	}
	// 从ID索引表中移除
	std::map<CHAR_GUID, Character*>::iterator iterID = mCharacterIDList.find(character->getGUID());
	if (iterID != mCharacterIDList.end())
	{
		mCharacterIDList.erase(iterID);
	}
}

void CharacterManager::destroyCharacter(const CHAR_GUID& guid)
{
	Character* character = getCharacter(guid);
	if (character == NULL)
	{
		return;
	}
	removeCharacterFromList(character);
	TRACE_DELETE(character);
}

void CharacterManager::destroyCharacter(const std::string& name)
{
	Character* character = getCharacter(name);
	if (character == NULL)
	{
		return;
	}
	removeCharacterFromList(character);
	TRACE_DELETE(character);
}

void CharacterManager::notifyCharacterIDChanged(const CHAR_GUID& oldID)
{
	std::map<CHAR_GUID, Character*>::iterator iterID = mCharacterIDList.find(oldID);
	if (iterID != mCharacterIDList.end())
	{
		Character* character = iterID->second;
		mCharacterIDList.erase(iterID);
		mCharacterIDList.insert(std::pair<CHAR_GUID, Character*>(character->getGUID(), character));
	}
}

void CharacterManager::notifyCharacterNameChanged(const std::string& oldName)
{
	Character* character = NULL;
	std::map<std::string, Character*>::iterator iterChar = mCharacterList.find(oldName);
	if (iterChar != mCharacterList.end())
	{
		character = iterChar->second;
		mCharacterList.erase(iterChar);
		mCharacterList.insert(std::pair<std::string, Character*>(character->getName(), character));
	}
	if (character != NULL)
	{
		std::map<CHARACTER_TYPE, std::map<std::string, Character*> >::iterator iterType = mCharacterTypeList.find(character->getType());
		if (iterType != mCharacterTypeList.end())
		{
			std::map<std::string, Character*>::iterator iterTypeChar = iterType->second.find(oldName);
			if (iterTypeChar != iterType->second.end())
			{
				iterType->second.erase(iterTypeChar);
				iterType->second.insert(std::pair<std::string, Character*>(character->getName(), character));
			}
		}
	}
}
