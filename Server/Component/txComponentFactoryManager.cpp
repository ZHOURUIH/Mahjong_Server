#include "txComponentFactoryManager.h"
#include "txComponentFactory.h"
#include "txComponentHeader.h"

void txComponentFactoryManager::init()
{
	// 基本组件类型
}

void txComponentFactoryManager::destory()
{
	txMap<std::string, txComponentFactoryBase*>::iterator iter = mFactoryList.begin();
	txMap<std::string, txComponentFactoryBase*>::iterator iterEnd = mFactoryList.end();
	FOR_STL(mFactoryList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mFactoryList);
	mFactoryList.clear();
}

txComponentFactoryBase* txComponentFactoryManager::getFactory(const std::string& type)
{
	txMap<std::string, txComponentFactoryBase*>::iterator iter = mFactoryList.find(type);
	if (iter != mFactoryList.end())
	{
		return iter->second;
	}
	return NULL;
}