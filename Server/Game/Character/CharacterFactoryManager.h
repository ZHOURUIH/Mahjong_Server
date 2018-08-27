#ifndef _CHARACTER_FACTORY_MANAGER_H_
#define _CHARACTER_FACTORY_MANAGER_H_

#include "Utility.h"
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
		auto iter = mFactoryList.begin();
		auto iterEnd = mFactoryList.end();
		FOR(mFactoryList, ; iter != iterEnd; ++iter)
		{
			TRACE_DELETE(iter->second);
		}
		END(mFactoryList);
		mFactoryList.clear();
	}
	CharacterFactoryBase* getFactory(CHARACTER_TYPE type)
	{
		return mFactoryList.tryGet(type, NULL);
	}
	txMap<CHARACTER_TYPE, CharacterFactoryBase*>& getFactoryList() { return mFactoryList; }
protected:
	template<typename O>
	CharacterFactoryBase* addFactory(CHARACTER_TYPE type)
	{
		CharacterFactoryBase* factory = CharacterFactoryBase::createFactory<O>(type);
		mFactoryList.insert(factory->getType(), factory);
		return factory;
	}
protected:
	txMap<CHARACTER_TYPE, CharacterFactoryBase*> mFactoryList;
};

#endif
