#ifndef _TX_COMPONENT_FACTORY_MANAGER_H_
#define _TX_COMPONENT_FACTORY_MANAGER_H_

#include "txUtility.h"
#include "txComponentFactory.h"

#define ADD_COMPONENT_FACTORY(t) addFactory<txComponentFactory<t> >(TOSTRING(t));

class txComponentFactoryBase;
class txComponentFactoryManager
{
public:
	virtual ~txComponentFactoryManager(){destory();}
	virtual void init();
	virtual void destory()
	{
		std::map<std::string, txComponentFactoryBase*>::iterator iter = mFactoryList.begin();
		std::map<std::string, txComponentFactoryBase*>::iterator iterEnd = mFactoryList.end();
		for (; iter != iterEnd; ++iter)
		{
			TRACE_DELETE(iter->second);
		}
		mFactoryList.clear();
	}
	txComponentFactoryBase* getFactory(const std::string& type)
	{
		std::map<std::string, txComponentFactoryBase*>::iterator iter = mFactoryList.find(type);
		if (iter != mFactoryList.end())
		{
			return iter->second;
		}
		return NULL;
	}
	std::map<std::string, txComponentFactoryBase*>& getFactoryList() { return mFactoryList; }
protected:
	template<typename O>
	txComponentFactoryBase* addFactory(const std::string& type)
	{
		txComponentFactoryBase* factory = txComponentFactoryBase::createFactory<O>(type);
		mFactoryList.insert(std::make_pair(factory->getType(), factory));
		return factory;
	}
protected:
	std::map<std::string, txComponentFactoryBase*> mFactoryList;
};

#endif
