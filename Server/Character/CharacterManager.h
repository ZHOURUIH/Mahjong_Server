#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "txCommandReceiver.h"

#include "CharacterDefine.h"

class Character;
class CharacterMyself;
class CharacterFactoryManager;
class CharacterManager : public txCommandReceiver
{
public:
	CharacterManager();
	virtual ~CharacterManager(){ destroy(); }

	void init(){}
	void update(const float& elapsedTime);
	void destroy();
	bool isLocked() { return mLockUpdate; }
	void lock() { mLockUpdate = true; }
	void unlock() { mLockUpdate = false; }
	Character* createCharacter(const std::string& name, const CHARACTER_TYPE& type, const CHAR_GUID& guid, const CLIENT_GUID& client);
	void destroyCharacter(const std::string& name);
	void destroyCharacter(const CHAR_GUID& guid);
	Character* getCharacter(const std::string& name)
	{
		std::map<std::string, Character*>::iterator itrFind = mCharacterList.find(name);
		if (mCharacterList.end() != itrFind)
		{
			return itrFind->second;
		}
		return NULL;
	}
	Character* getCharacter(const CHAR_GUID& characterID)
	{
		std::map<CHAR_GUID, Character*>::iterator iterID = mCharacterIDList.find(characterID);
		if (iterID != mCharacterIDList.end())
		{
			return iterID->second;
		}
		return NULL;
	}
	void getCharacterListByType(const CHARACTER_TYPE& type, std::map<std::string, Character*>& characterList)
	{
		std::map<CHARACTER_TYPE, std::map<std::string, Character*> >::iterator iterType = mCharacterTypeList.find(type);
		if (iterType != mCharacterTypeList.end())
		{
			characterList = iterType->second;
		}
	}
	void notifyCharacterIDChanged(const CHAR_GUID& oldID);
	void notifyCharacterNameChanged(const std::string& oldName);
	bool isCharacterLogin(const CHAR_GUID& guid)
	{
		std::map<CHAR_GUID, Character*>::iterator iter = mCharacterIDList.find(guid);
		return iter != mCharacterIDList.end();
	}
protected:
	void addCharacterToList(Character* character);
	void removeCharacterFromList(Character* character);
protected:
	std::map<std::string, Character*> mCharacterList;
	std::map<CHARACTER_TYPE, std::map<std::string, Character*> > mCharacterTypeList;	// 角色分类列表
	std::map<CHAR_GUID, Character*> mCharacterIDList;		// 角色ID索引表
	bool mLockUpdate;
	CharacterFactoryManager* mCharacterFactoryManager;
};

#endif