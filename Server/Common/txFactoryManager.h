﻿#if 0
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
		typename auto iter = mFactoryList.begin();
		typename auto iterEnd = mFactoryList.end();
		FOR_STL (mFactoryList, ; iter != iterEnd; ++iter)
		{
			TRACE_DELETE(iter->second);
		}
		END_FOR_STL(mFactoryList);
		mFactoryList.clear();
	}
	Base* getFactory(const T& type)
	{
		return mFactoryList.tryGet(type, NULL);
	}
	txMap<T, Base*>& getFactoryList() { return mFactoryList; }
protected:
	template<typename O>
	Base* addFactory(const T& type)
	{
		Base* factory = Base::createFactory<O>(type);
		mFactoryList.insert(factory->getType(), factory);
		return factory;
	}
protected:
	txMap<T, Base*> mFactoryList;};

#endif
#endif