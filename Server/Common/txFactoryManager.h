#if 0
#ifndef _TX_FACTORY_MANAGER_H_
#define _TX_FACTORY_MANAGER_H_

#include "txUtility.h"

template<typename T, typename Base>
class txFactoryManager
{
public:
	txFactoryManager(){}
	virtual ~txFactoryManager(){destory();}
	virtual void init() = 0;
	virtual void destory()
	{
		typename std::map<T, Base*>::iterator iter = mFactoryList.begin();
		typename std::map<T, Base*>::iterator iterEnd = mFactoryList.end();
		for (; iter != iterEnd; ++iter)
		{
			TRACE_DELETE(iter->second);
		}
		mFactoryList.clear();
	}
	Base* getFactory(const T& type)
	{
		typename std::map<T, Base*>::iterator iter = mFactoryList.find(type);
		if (iter != mFactoryList.end())
		{
			return iter->second;
		}
		return NULL;
	}
	std::map<T, Base*>& getFactoryList() { return mFactoryList; }
protected:
	template<typename O>
	Base* addFactory(const T& type)
	{
		Base* factory = Base::createFactory<O>(type);
		mFactoryList.insert(std::make_pair(factory->getType(), factory));
		return factory;
	}
protected:
	std::map<T, Base*> mFactoryList;};

#endif
#endif