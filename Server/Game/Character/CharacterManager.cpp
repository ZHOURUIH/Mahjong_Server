#include "Utility.h"

#include "CharacterManager.h"
#include "CharacterFactory.h"
#include "CharacterData.h"
#include "CharacterPlayer.h"
#include "CharacterFactoryManager.h"

CharacterManager::CharacterManager(const std::string& name)
:
FrameComponent(name),
mLockUpdate(false)
{
	mCharacterFactoryManager = TRACE_NEW(CharacterFactoryManager, mCharacterFactoryManager);
}

void CharacterManager::init()
{
	mCharacterFactoryManager->init();
}

void CharacterManager::update(float elapsedTime)
{
	if (isLocked())
	{
		return;
	}
	auto characterBegin = mCharacterList.begin();
	auto characterEnd = mCharacterList.end();
	FOR_STL(mCharacterList, ; characterBegin != characterEnd; ++characterBegin)
	{
		Character* character = characterBegin->second;
		if (NULL != character)
		{
			character->update(elapsedTime);
		}
	}
	END_FOR_STL(mCharacterList);
}

void CharacterManager::destroy()
{
	auto iterType = mCharacterTypeList.begin();
	auto iterTypeEnd = mCharacterTypeList.end();
	FOR_STL(mCharacterTypeList, ; iterType != iterTypeEnd; ++iterType)
	{
		auto characterFactory = mCharacterFactoryManager->getFactory(iterType->first);
		auto iterChar = iterType->second.begin();
		auto iterCharEnd = iterType->second.end();
		FOR_STL(iterType->second, ; iterChar != iterCharEnd; ++iterChar)
		{
			characterFactory->destroyCharacter(iterChar->second);
		}
		END_FOR_STL(iterType->second);
	}
	END_FOR_STL(mCharacterTypeList);
	mCharacterList.clear();
	mCharacterTypeList.clear();
}

Character* CharacterManager::createCharacter(const std::string& name, CHARACTER_TYPE type, CHAR_GUID guid, CLIENT_GUID client)
{
	if (mCharacterList.find(name) != mCharacterList.end())
	{
		LOG_ERROR("error : there is a character named : %s! can not create again!", name.c_str());
		return NULL;
	}
	CharacterFactoryBase* factory = mCharacterFactoryManager->getFactory(type);
	if(NULL == factory)
	{
		LOG_ERROR("error : can not find CharacterFactory ,type %d ", (int)type);
		return NULL;
	}

	Character* newChar = factory->createCharacter(name);
	if (newChar != NULL)
	{
		if (type == CT_PLAYER)
		{
			static_cast<CharacterPlayer*>(newChar)->setClientGUID(client);
		}
		newChar->init(guid);
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
	mCharacterList.insert(character->getName(), character);
	// 加入到角色分类列表
	auto iterType = mCharacterTypeList.find(character->getType());
	if (iterType != mCharacterTypeList.end())
	{
		iterType->second.insert(character->getName(), character);
	}
	else
	{
		txMap<std::string, Character*> characterMap;
		characterMap.insert(character->getName(), character);
		mCharacterTypeList.insert(character->getType(), characterMap);
	}
	// 加入ID索引表
	int characterID = character->getGUID();
	if (mCharacterIDList.find(characterID) != mCharacterIDList.end())
	{
		LOG_ERROR("error : there is a character id : %d", characterID);
	}
	mCharacterIDList.insert(characterID, character);
}

void CharacterManager::removeCharacterFromList(Character* character)
{
	if (character == NULL)
	{
		return;
	}
	// 从全部角色列表中移除
	mCharacterList.tryErase(character->getName());
	// 从角色分类列表中移除
	auto iterType = mCharacterTypeList.find(character->getType());
	if (iterType != mCharacterTypeList.end())
	{
		iterType->second.tryErase(character->getName());
	}
	// 从ID索引表中移除
	mCharacterIDList.tryErase(character->getGUID());
}

void CharacterManager::destroyCharacter(CHAR_GUID guid)
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

void CharacterManager::notifyCharacterIDChanged(CHAR_GUID oldID)
{
	auto iterID = mCharacterIDList.find(oldID);
	if (iterID != mCharacterIDList.end())
	{
		Character* character = iterID->second;
		mCharacterIDList.erase(iterID);
		mCharacterIDList.insert(character->getGUID(), character);
	}
}

void CharacterManager::notifyCharacterNameChanged(const std::string& oldName)
{
	Character* character = NULL;
	auto iterChar = mCharacterList.find(oldName);
	if (iterChar != mCharacterList.end())
	{
		character = iterChar->second;
		mCharacterList.erase(iterChar);
		mCharacterList.insert(character->getName(), character);
	}
	if (character != NULL)
	{
		auto iterType = mCharacterTypeList.find(character->getType());
		if (iterType != mCharacterTypeList.end())
		{
			auto iterTypeChar = iterType->second.find(oldName);
			if (iterTypeChar != iterType->second.end())
			{
				iterType->second.erase(iterTypeChar);
				iterType->second.insert(character->getName(), character);
			}
		}
	}
}
