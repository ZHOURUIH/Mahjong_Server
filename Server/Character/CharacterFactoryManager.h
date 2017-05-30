#ifndef _CHARACTER_FACTORY_MANAGER_H_
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
		std::map<CHARACTER_TYPE, CharacterFactoryBase*>::iterator iter = mFactoryList.begin();
		std::map<CHARACTER_TYPE, CharacterFactoryBase*>::iterator iterEnd = mFactoryList.end();
		for (; iter != iterEnd; ++iter)
		{
			TRACE_DELETE(iter->second);
		}
		mFactoryList.clear();
	}
	CharacterFactoryBase* getFactory(const CHARACTER_TYPE& type)
	{
		std::map<CHARACTER_TYPE, CharacterFactoryBase*>::iterator iter = mFactoryList.find(type);
		if (iter != mFactoryList.end())
		{
			return iter->second;
		}
		return NULL;
	}
	std::map<CHARACTER_TYPE, CharacterFactoryBase*>& getFactoryList() { return mFactoryList; }
protected:
	template<typename O>
	CharacterFactoryBase* addFactory(const CHARACTER_TYPE& type)
	{
		CharacterFactoryBase* factory = CharacterFactoryBase::createFactory<O>(type);
		mFactoryList.insert(std::make_pair(factory->getType(), factory));
		return factory;
	}
protected:
	std::map<CHARACTER_TYPE, CharacterFactoryBase*> mFactoryList;
};

#endif
