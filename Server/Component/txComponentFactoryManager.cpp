#include "txComponentFactoryManager.h"
#include "txComponentFactory.h"
#include "txComponentHeader.h"

void txComponentFactoryManager::init()
{
	// 基本组件类型
}

void txComponentFactoryManager::destory()
{
	auto iter = mFactoryList.begin();
	auto iterEnd = mFactoryList.end();
	FOR_STL(mFactoryList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mFactoryList);
	mFactoryList.clear();
}

txComponentFactoryBase* txComponentFactoryManager::getFactory(const std::string& type)
{
	return mFactoryList.tryGet(type, NULL);
}