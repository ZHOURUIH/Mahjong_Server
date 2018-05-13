#include "ServerFramework.h"
#include "CommandHeader.h"
#include "Character.h"
#include "CharacterData.h"
#include "Utility.h"

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
	// 鍒濆鍖栭粯璁ょ粍浠?
}

void Character::update(const float& elaspedTime)
{
	// 鍏堟洿鏂拌嚜宸辩殑鎵€鏈夌粍浠?
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