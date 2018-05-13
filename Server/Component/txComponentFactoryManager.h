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
	virtual void destory();
	txComponentFactoryBase* getFactory(const std::string& type);
	txMap<std::string, txComponentFactoryBase*>& getFactoryList() { return mFactoryList; }
protected:
	template<typename O>
	txComponentFactoryBase* addFactory(const std::string& type)
	{
		txComponentFactoryBase* factory = txComponentFactoryBase::createFactory<O>(type);
		mFactoryList.insert(factory->getType(), factory);
		return factory;
	}
protected:
	txMap<std::string, txComponentFactoryBase*> mFactoryList;
};

#endif
