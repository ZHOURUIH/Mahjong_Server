#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "CharacterDefine.h"
#include "txComponentOwner.h"

class CharacterData;
class Character : public txComponentOwner
{
public:
	Character(const CHARACTER_TYPE& type, const std::string& name);
	virtual ~Character(){ destroy(); }
	virtual void init(const CHAR_GUID& guid);
	virtual void initComponents();
	virtual void update(const float& elaspedTime);
	void destroy();
	virtual void notifyComponentChanged(txComponent* component);
	CharacterData* getCharacterData(){ return mCharacterData; }
	const CHARACTER_TYPE& getType() { return mCharacterType; }
	const CHAR_GUID& getGUID() { return mGUID; }
protected:
	CHARACTER_TYPE mCharacterType;	// 角色类型
	CharacterData* mCharacterData;	//玩家数据
	CHAR_GUID mGUID;				// 角色ID
};

#endif