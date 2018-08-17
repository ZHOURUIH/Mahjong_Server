#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "CharacterDefine.h"
#include "txComponentOwner.h"

class CharacterData;
class Character : public txComponentOwner
{
public:
	Character(CHARACTER_TYPE type, const std::string& name);
	virtual ~Character(){ destroy(); }
	virtual void init(CHAR_GUID guid);
	virtual void initComponents();
	virtual void update(float elaspedTime);
	void destroy();
	virtual void notifyComponentChanged(txComponent* component);
	CharacterData* getCharacterData(){ return mCharacterData; }
	CHARACTER_TYPE getType() { return mCharacterType; }
	CHAR_GUID getGUID() { return mGUID; }
protected:
	CHARACTER_TYPE mCharacterType;	// 角色类型
	CharacterData* mCharacterData;	//玩家数据
	CHAR_GUID mGUID;				// 角色ID
};

#endif