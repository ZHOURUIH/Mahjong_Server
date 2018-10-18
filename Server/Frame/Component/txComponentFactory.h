﻿#ifndef _TX_COMPONENT_FACTORY_BASE_H_
#define _TX_COMPONENT_FACTORY_BASE_H_

#include "Utility.h"
#include "txComponent.h"
#include "GameLog.h"

class txComponentFactoryBase
{
public:
	txComponentFactoryBase(const std::string& type)
		:
		mType(type),
		mCount(0),
		mNameCount(0)
	{}
	virtual ~txComponentFactoryBase()
	{
		if (mCount != 0)
		{
			LOG_ERROR("there are components left! count : " + StringUtility::intToString(mCount) + ", type : " + mType);
		}
	}
	virtual txComponent* createComponent(const std::string& name) = 0;
	template <typename T>
	static txComponentFactoryBase* createFactory(const std::string& type)
	{
		T* newFacotry = TRACE_NEW(T, newFacotry, type);
		return newFacotry;
	}
	void destroyComponent(txComponent*& component)
	{
		if (component == NULL)
		{
			LOG_ERROR("can not destroy NULL character component!");
			return;
		}
		if (component->getType() != mType)
		{
			LOG_ERROR("destroy wrong type of character component! factory type : " + mType  + ", component type : " + component->getType());
		}
		TRACE_DELETE(component);
		--mCount;
	}
	const std::string& getType() { return mType; }
	std::string createUniqueName()
	{
		return mType + StringUtility::intToString(mNameCount++);
	}
protected:
	std::string mType;
	int mCount;
	int mNameCount;	// 用于生成唯一名字
};

template<class T>
class txComponentFactory : public txComponentFactoryBase
{
public:
	txComponentFactory(const std::string& type)
	:
	txComponentFactoryBase(type)
	{}
	txComponent* createComponent(const std::string& name)
	{
		T* newComponent = NULL;
		if (name == EMPTY_STRING)
		{
			TRACE_NEW(T, newComponent, mType, createUniqueName());
		}
		else
		{
			TRACE_NEW(T, newComponent, mType, name);
		}
		++mCount;
		return newComponent;
	}
};

#endif