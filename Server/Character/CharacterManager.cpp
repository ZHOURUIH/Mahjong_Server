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
	txMap<std::string, Character*>::iterator characterBegin = mCharacterList.begin();
	txMap<std::string, Character*>::iterator characterEnd = mCharacterList.end();
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
	txMap<CHARACTER_TYPE, txMap<std::string, Character*> >::iterator iterType = mCharacterTypeList.begin();
	txMap<CHARACTER_TYPE, txMap<std::string, Character*> >::iterator iterTypeEnd = mCharacterTypeList.end();
	FOR_STL(mCharacterTypeList, ; iterType != iterTypeEnd; ++iterType)
	{
		CharacterFactoryBase* characterFactory = mCharacterFactoryManager->getFactory(iterType->first);
		txMap<std::string, Character*>::iterator iterChar = iterType->second.begin();
		txMap<std::string, Character*>::iterator iterCharEnd = iterType->second.end();
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

Character* CharacterManager::createCharacter(const std::string& name, const CHARACTER_TYPE& type, const CHAR_GUID& guid, const CLIENT_GUID& client)
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
	txMap<CHARACTER_TYPE, txMap<std::string, Character*> >::iterator iterType = mCharacterTypeList.find(character->getType());
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
		GAME_ERROR("error : there is a character id : %d", characterID);
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
	txMap<std::string, Character*>::iterator iter = mCharacterList.find(character->getName());
	if (iter != mCharacterList.end())
	{
		mCharacterList.erase(iter);
	}
	// 从角色分类列表中移除
	txMap<CHARACTER_TYPE, txMap<std::string, Character*> >::iterator iterType = mCharacterTypeList.find(character->getType());
	if (iterType != mCharacterTypeList.end())
	{
		txMap<std::string, Character*>::iterator iterChar = iterType->second.find(character->getName());
		if (iterChar != iterType->second.end())
		{
			iterType->second.erase(iterChar);
		}
	}
	// 从ID索引表中移除
	txMap<CHAR_GUID, Character*>::iterator iterID = mCharacterIDList.find(character->getGUID());
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
	txMap<CHAR_GUID, Character*>::iterator iterID = mCharacterIDList.find(oldID);
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
	txMap<std::string, Character*>::iterator iterChar = mCharacterList.find(oldName);
	if (iterChar != mCharacterList.end())
	{
		character = iterChar->second;
		mCharacterList.erase(iterChar);
		mCharacterList.insert(character->getName(), character);
	}
	if (character != NULL)
	{
		txMap<CHARACTER_TYPE, txMap<std::string, Character*> >::iterator iterType = mCharacterTypeList.find(character->getType());
		if (iterType != mCharacterTypeList.end())
		{
			txMap<std::string, Character*>::iterator iterTypeChar = iterType->second.find(oldName);
			if (iterTypeChar != iterType->second.end())
			{
				iterType->second.erase(iterTypeChar);
				iterType->second.insert(character->getName(), character);
			}
		}
	}
}
