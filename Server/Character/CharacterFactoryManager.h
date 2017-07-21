﻿#ifndef _CHARACTER_FACTORY_MANAGER_H_
#define _CHARACTER_FACTORY_MANAGER_H_

#include "txUtility.h"
#include "CharacterFactory.h"
#include "CharacterDefine.h"

class CharacterFactoryBase;
class CharacterFactoryManager
{
public:
	virtual ~CharacterFactoryManager(){destory();}
	virtual void init();
	virtual void destory()
	{
		txMap<CHARACTER_TYPE, CharacterFactoryBase*>::iterator iter = mFactoryList.begin();
		txMap<CHARACTER_TYPE, CharacterFactoryBase*>::iterator iterEnd = mFactoryList.end();
		FOR_STL(mFactoryList, ; iter != iterEnd; ++iter)
		{
			TRACE_DELETE(iter->second);
		}
		END_FOR_STL(mFactoryList);
		mFactoryList.clear();
	}
	CharacterFactoryBase* getFactory(const CHARACTER_TYPE& type)
	{
		txMap<CHARACTER_TYPE, CharacterFactoryBase*>::iterator iter = mFactoryList.find(type);
		if (iter != mFactoryList.end())
		{
			return iter->second;
		}
		return NULL;
	}
	txMap<CHARACTER_TYPE, CharacterFactoryBase*>& getFactoryList() { return mFactoryList; }
protected:
	template<typename O>
	CharacterFactoryBase* addFactory(const CHARACTER_TYPE& type)
	{
		CharacterFactoryBase* factory = CharacterFactoryBase::createFactory<O>(type);
		mFactoryList.insert(factory->getType(), factory);
		return factory;
	}
protected:
	txMap<CHARACTER_TYPE, CharacterFactoryBase*> mFactoryList;
};

#endif
