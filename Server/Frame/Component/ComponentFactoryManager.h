#ifndef _COMPONENT_FACTORY_MANAGER_H_
#define _COMPONENT_FACTORY_MANAGER_H_

#include "Utility.h"
#include "txComponentFactory.h"
#include "FrameComponent.h"

#define ADD_COMPONENT_FACTORY(t) addFactory<txComponentFactory<t> >(TOSTRING(t));

class txComponentFactoryBase;
class ComponentFactoryManager : public FrameComponent
{
public:
	ComponentFactoryManager(const string& name)
		:FrameComponent(name) {}
	virtual ~ComponentFactoryManager(){destory();}
	virtual void init();
	void destory();
	txComponentFactoryBase* getFactory(const string& type);
	txMap<string, txComponentFactoryBase*>& getFactoryList() { return mFactoryList; }
protected:
	template<typename O>
	txComponentFactoryBase* addFactory(const string& type)
	{
		txComponentFactoryBase* factory = txComponentFactoryBase::createFactory<O>(type);
		mFactoryList.insert(factory->getType(), factory);
		return factory;
	}
protected:
	txMap<string, txComponentFactoryBase*> mFactoryList;
};

#endif
