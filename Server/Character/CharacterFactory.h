#ifndef _CHARACTER_FACTORY_H_
#define _CHARACTER_FACTORY_H_

#include "CharacterDefine.h"
#include "Character.h"
#include "Utility.h"
#include "GameLog.h"

class CharacterFactoryBase
{
public:
	CharacterFactoryBase(const CHARACTER_TYPE& type)
		:
		mType(type),
		mCurCount(0)
	{}
	virtual ~CharacterFactoryBase()
	{
		if (mCurCount != 0)
		{
			GAME_ERROR("error : there is character left! count : %d", mCurCount);
		}
	}
	virtual Character* createCharacter(const std::string& name) = 0;
	void destroyCharacter(Character*& character)
	{
		if (character == NULL)
		{
			GAME_ERROR("error : can not destroy NULL character!");
			return;
		}
		if (character->getType() != mType)
		{
			GAME_ERROR("error : destroy wrong type character! factory type : %d, character type : %d", (int)mType, (int)character->getType());
		}
		TRACE_DELETE(character);
		--mCurCount;
	}
	template<typename T>
	static CharacterFactoryBase* createFactory(const CHARACTER_TYPE& type)
	{
		T* factory = TRACE_NEW(T, factory, type);
		return factory;
	}
	const CHARACTER_TYPE& getType() { return mType; }
protected:
	CHARACTER_TYPE mType;
	int mCurCount;
};

template<class T>
class CharacterFactory : public CharacterFactoryBase
{
public:
	CharacterFactory(const CHARACTER_TYPE& type)
		:
		CharacterFactoryBase(type)
	{}
	virtual Character* createCharacter(const std::string& name)
	{
		T* newCharacter = TRACE_NEW(T, newCharacter, mType, name);
		++mCurCount;
		return newCharacter;
	}
};

#endif