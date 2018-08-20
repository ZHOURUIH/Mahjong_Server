#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "FrameComponent.h"

#include "CharacterDefine.h"

class Character;
class CharacterMyself;
class CharacterFactoryManager;
class CharacterManager : public FrameComponent
{
public:
	CharacterManager(const std::string& name);
	virtual ~CharacterManager(){ destroy(); }
	virtual void init();
	virtual void update(float elapsedTime);
	void destroy();
	bool isLocked() { return mLockUpdate; }
	void lock() { mLockUpdate = true; }
	void unlock() { mLockUpdate = false; }
	Character* createCharacter(const std::string& name, CHARACTER_TYPE type, CHAR_GUID guid, CLIENT_GUID client);
	void destroyCharacter(const std::string& name);
	void destroyCharacter(CHAR_GUID guid);
	Character* getCharacter(const std::string& name)
	{
		return mCharacterList.tryGet(name, NULL);
	}
	Character* getCharacter(CHAR_GUID characterID)
	{
		return mCharacterIDList.tryGet(characterID, NULL);
	}
	void getCharacterListByType(CHARACTER_TYPE type, txMap<std::string, Character*>& characterList)
	{
		characterList = mCharacterTypeList.tryGet(type, txMap<std::string, Character*>());
	}
	void notifyCharacterIDChanged(CHAR_GUID oldID);
	void notifyCharacterNameChanged(const std::string& oldName);
	bool isCharacterLogin(CHAR_GUID guid)
	{
		return mCharacterIDList.find(guid) != mCharacterIDList.end();
	}
protected:
	void addCharacterToList(Character* character);
	void removeCharacterFromList(Character* character);
protected:
	txMap<std::string, Character*> mCharacterList;
	txMap<CHARACTER_TYPE, txMap<std::string, Character*> > mCharacterTypeList;	// 角色分类列表
	txMap<CHAR_GUID, Character*> mCharacterIDList;		// 角色ID索引表
	bool mLockUpdate;
	CharacterFactoryManager* mCharacterFactoryManager;
};

#endif