#include "ServerFramework.h"
#include "CommandHeader.h"
#include "Character.h"
#include "CharacterData.h"
#include "txMemeryTrace.h"

Character::Character(const CHARACTER_TYPE& type, const std::string& name)
:
txComponentOwner(name),
mCharacterType(type),
mCharacterData(NULL),
mGUID(INVALID_ID)
{}

void Character::init(const CHAR_GUID& guid)
{
	if (mCharacterData == NULL)
	{
		mCharacterData = TRACE_NEW(CharacterData, mCharacterData);
	}
	mGUID = guid;
	mCharacterData->mName = mName;
	mCharacterData->mGUID = mGUID;
	initComponents();
}

void Character::initComponents()
{
	// 初始化默认组件
}

void Character::update(const float& elaspedTime)
{
	// 先更新自己的所有组件
	txComponentOwner::updateComponents(elaspedTime);
}

void Character::destroy()
{
	TRACE_DELETE(mCharacterData);
}

void Character::notifyComponentChanged(txComponent* component)
{
	;
}