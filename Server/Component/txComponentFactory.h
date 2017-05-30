#ifndef _TX_COMPONENT_FACTORY_BASE_H_
#define _TX_COMPONENT_FACTORY_BASE_H_

#include "txUtility.h"
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
			GAME_ERROR("error : there are components left! count : %d, type : %s", mCount, mType.c_str());
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
			GAME_ERROR("error : can not destroy NULL character component!");
			return;
		}
		if (component->getType() != mType)
		{
			GAME_ERROR("error : destroy wrong type of character component! factory type : %s, component type : %s", mType.c_str(), component->getType().c_str());
		}
		TRACE_DELETE(component);
		--mCount;
	}
	const std::string& getType() { return mType; }
	std::string createUniqueName()
	{
		return mType + txUtility::intToString(mNameCount++);
	}
protected:
	std::string mType;
	int mCount;
	int mNameCount;	// ��������Ψһ����
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
			newComponent = TRACE_NEW(T, newComponent, mType, createUniqueName());
		}
		else
		{
			newComponent = TRACE_NEW(T, newComponent, mType, name);
		}
		++mCount;
		return newComponent;
	}
};

#endif