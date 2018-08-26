#ifndef _TX_COMPONENT_FACTORY_MANAGER_H_
#define _TX_COMPONENT_FACTORY_MANAGER_H_

#include "Utility.h"
#include "txComponentFactory.h"
#include "FrameComponent.h"

#define ADD_COMPONENT_FACTORY(t) addFactory<txComponentFactory<t> >(TOSTRING(t));

class txComponentFactoryBase;
class txComponentFactoryManager : public FrameComponent
{
public:
	txComponentFactoryManager(const std::string& name)
		:FrameComponent(name) {}
	virtual ~txComponentFactoryManager(){destory();}
	virtual void init();
	void destory();
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
