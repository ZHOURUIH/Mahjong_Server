#include "ComponentFactoryManager.h"
#include "txComponentFactory.h"
#include "txComponentHeader.h"

void ComponentFactoryManager::init()
{
	// 基本组件类型
}

void ComponentFactoryManager::destory()
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

txComponentFactoryBase* ComponentFactoryManager::getFactory(const string& type)
{
	return mFactoryList.tryGet(type, NULL);
}