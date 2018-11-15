#include "txComponentHeader.h"
#include "txComponentFactory.h"
#include "ComponentFactoryManager.h"
#include "GameLog.h"

txMap<string, txComponent*> ComponentOwner::EMPTY_COMPONENT_MAP;

void ComponentOwner::updatePreComponent(float elapsedTime)
{
	if (mAllComponentList.size() == 0)
	{
		return;
	}
	int rootComponentCount = mRootComponentList.size();
	if (mUsePreLateUpdate)
	{
		// 预更新基础类型组件
		FOR(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
		{
			txComponent* component = mRootComponentList[i];
			if (component->isNeedPreUpdate())
			{
				if (component != NULL && component->isActive() && !component->isLockOneFrame())
				{
					component->preUpdate(elapsedTime);
				}
			}
		}
		END(mRootComponentList);
	}
	// 更新基础类型组件
	FOR(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
	{
		txComponent* component = mRootComponentList[i];
		if (component->isNeedPreUpdate())
		{
			if (component != NULL && component->isActive() && !component->isLockOneFrame())
			{
				component->update(elapsedTime);
			}
		}
	}
	END(mRootComponentList);
	if (mUsePreLateUpdate)
	{
		// 补充更新基础类型组件
		FOR(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
		{
			txComponent* component = mRootComponentList[i];
			if (component->isNeedPreUpdate())
			{
				if (component != NULL && component->isActive())
				{
					// 如果组件被锁定了一帧,则不更新,解除锁定
					if (component->isLockOneFrame())
					{
						component->setLockOneFrame(false);
					}
					else
					{
						component->lateUpdate(elapsedTime);
					}
				}
			}
		}
		END(mRootComponentList);
	}
}

void ComponentOwner::updateComponents(float elapsedTime)
{
	if (mAllComponentList.size() == 0)
	{
		return;
	}
	int rootComponentCount = mRootComponentList.size();
	if (mUsePreLateUpdate)
	{
		// 预更新基础类型组件
		FOR(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
		{
			txComponent* component = mRootComponentList[i];
			if (!component->isNeedPreUpdate())
			{
				if (component != NULL && component->isActive() && !component->isLockOneFrame())
				{
					component->preUpdate(elapsedTime);
				}
			}
		}
		END(mRootComponentList);
	}
	// 更新基础类型组件
	FOR(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
	{
		txComponent* component = mRootComponentList[i];
		if (!component->isNeedPreUpdate())
		{
			if (component != NULL && component->isActive() && !component->isLockOneFrame())
			{
				component->update(elapsedTime);
			}
		}
	}
	END(mRootComponentList);
	if (mUsePreLateUpdate)
	{
		// 补充更新基础类型组件
		FOR(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
		{
			txComponent* component = mRootComponentList[i];
			if (!component->isNeedPreUpdate())
			{
				if (component != NULL && component->isActive())
				{
					// 如果组件被锁定了一帧,则不更新,解除锁定
					if (component->isLockOneFrame())
					{
						component->setLockOneFrame(false);
					}
					else
					{
						component->lateUpdate(elapsedTime);
					}
				}
			}
		}
		END(mRootComponentList);
	}
}

void ComponentOwner::notifyComponentAttached(txComponent* component)
{
	if (component == NULL)
	{
		return;
	}
	if (!mAllComponentList.contains(component->getName()))
	{
		addComponentToList(component);
	}
}

bool ComponentOwner::notifyComponentNameChanged(const string& oldName, txComponent* component)
{
	// 先查找是否有该名字的组件
	if (!mAllComponentList.contains(oldName))
	{
		return false;
	}
	// 再查找改名后会不会重名
	if (mAllComponentList.contains(component->getName()))
	{
		return false;
	}
	removeComponentFromList(component);
	addComponentToList(component);
	return true;
}

txComponent* ComponentOwner::createIndependentComponent(const string& name, const string& type, bool initComponent)
{
	// 查找工厂
	txComponentFactoryBase* factory = mComponentFactoryManager->getFactory(type);
	if (factory == NULL)
	{
		LOG_ERROR("can not find component factory! type : " + type);
		return NULL;
	}
	// 创建组件并且设置拥有者,然后初始化
	txComponent* component = factory->createComponent(name);
	if (initComponent && component != NULL)
	{
		component->init(NULL);
	}
	return component;
}

txComponent* ComponentOwner::addComponent(const string& name, const string& type)
{
	// 不能创建重名的组件
	if (mAllComponentList.contains(name))
	{
		LOG_ERROR("there is component named : " + name + " in the list");
		return NULL;
	}
	txComponent* component = createIndependentComponent(name, type, false);
	component->init(this);
	// 将组件加入列表
	addComponentToList(component);
	// 通知创建了组件
	notifyAddComponent(component);
	return component;
}

void ComponentOwner::destroyComponent(txComponent* component)
{
	// 后序遍历销毁组件,从最底层组件开始销毁,此处不能用引用获得子组件列表,因为在销毁组件过程中会对列表进行修改
	txVector<txComponent*> children = component->getChildComponentList();
	int childCount = children.size();
	FOR(children, int i = 0; i < childCount; ++i)
	{
		destroyComponent(children[i]);
	}
	END(children);
	mComponentFactoryManager->getFactory(component->getType())->destroyComponent(component);
}

void ComponentOwner::destroyComponent(const string& name)
{
	// 在总列表中查找
	auto component = mAllComponentList.tryGet(name, NULL);
	if (component != NULL)
	{
		destroyComponent(component);
	}
}

void ComponentOwner::destroyAllComponents()
{
	auto iterType = mAllComponentTypeList.begin();
	auto iterTypeEnd = mAllComponentTypeList.end();
	FOR(mAllComponentTypeList, ; iterType != iterTypeEnd; ++iterType)
	{
		txComponentFactoryBase* factory = mComponentFactoryManager->getFactory(iterType->first);
		if (factory == NULL)
		{
			continue;
		}
		// 因为在销毁过程中会修改列表,复制一份是为了避免迭代器失效
		auto componentList = iterType->second;
		auto iterCom = componentList.begin();
		auto iterComEnd = componentList.end();
		FOR(componentList, ; iterCom != iterComEnd; ++iterCom)
		{
			factory->destroyComponent(iterCom->second);
		}
		END(componentList);
	}
	END(mAllComponentTypeList);
}

txComponent* ComponentOwner::getFirstActiveComponentByBaseType(const string& type)
{
	txComponent* ret = NULL;
	auto iterBaseType = mAllComponentBaseTypeList.find(type);
	if (iterBaseType != mAllComponentBaseTypeList.end())
	{
		auto iterTypeCom = iterBaseType->second.begin();
		auto iterTypeComEnd = iterBaseType->second.end();
		FOR(iterBaseType->second, ; iterTypeCom != iterTypeComEnd; ++iterTypeCom)
		{
			txComponent* component = iterTypeCom->second;
			if (component->isActive() && !component->isLockOneFrame())
			{
				ret = component;
			}
		}
		END(iterBaseType->second);
	}
	return ret;
}

txComponent* ComponentOwner::getFirstActiveComponent(const string& type)
{
	txComponent* ret = NULL;
	auto iter = mAllComponentTypeList.find(type);
	if (iter != mAllComponentTypeList.end())
	{
		auto iterTypeCom = iter->second.begin();
		auto iterTypeComEnd = iter->second.end();
		FOR(iter->second, ; iterTypeCom != iterTypeComEnd; ++iterTypeCom)
		{
			txComponent* component = iterTypeCom->second;
			if (component->isActive() && !component->isLockOneFrame())
			{
				ret = component;
			}
		}
		END(iter->second);
	}
	return ret;
}

void ComponentOwner::addComponentToList(txComponent* component, int componentPos)
{
	const auto& name = component->getName();
	const auto& type = component->getType();
	const auto& baseType = component->getBaseType();

	// 如果没有父组件,则将组件放入第一级组件列表中
	if (component->getParentComponent() == NULL)
	{
		if (componentPos == -1)
		{
			mRootComponentList.push_back(component);
		}
		else
		{
			mRootComponentList.insert(mRootComponentList.begin() + componentPos, component);
		}
	}

	// 添加到组件列表中
	mAllComponentList.insert(name, component);

	// 添加到组件类型分组列表中
	auto iterType = mAllComponentTypeList.find(type);
	if (iterType != mAllComponentTypeList.end())
	{
		iterType->second.insert(name, component);
	}
	else
	{
		txMap<string, txComponent*> componentList;
		componentList.insert(name, component);
		mAllComponentTypeList.insert(type, componentList);
	}

	// 添加到基础组件类型分组列表中
	auto iterBaseType = mAllComponentBaseTypeList.find(baseType);
	if (iterBaseType != mAllComponentBaseTypeList.end())
	{
		iterBaseType->second.insert(name, component);
	}
	else
	{
		txMap<string, txComponent*> componentList;
		componentList.insert(name, component);
		mAllComponentBaseTypeList.insert(baseType, componentList);
	}
}

void ComponentOwner::removeComponentFromList(txComponent* component)
{
	// 从第一级组件列表中移除
	if (component->getParentComponent() == NULL)
	{
		int componentCount = mRootComponentList.size();
		FOR(mRootComponentList, int i = 0; i < componentCount; ++i)
		{
			if (mRootComponentList[i] == component)
			{
				mRootComponentList.erase(mRootComponentList.begin() + i);
				break;
			}
		}
		END(mRootComponentList);
	}

	// 从所有组件列表中移除
	const auto& componentName = component->getName();
	mAllComponentList.tryErase(componentName);

	// 从组件类型分组列表中移除
	const auto& realType = component->getType();
	auto iterType = mAllComponentTypeList.find(realType);
	if (iterType != mAllComponentTypeList.end())
	{
		iterType->second.tryErase(componentName);
	}

	// 从基础组件类型分组列表中移除
	const auto& baseType = component->getBaseType();
	auto iterBaseType = mAllComponentBaseTypeList.find(baseType);
	if (iterBaseType != mAllComponentBaseTypeList.end())
	{
		iterBaseType->second.tryErase(componentName);
	}
}