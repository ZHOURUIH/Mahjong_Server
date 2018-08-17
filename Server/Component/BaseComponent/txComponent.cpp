#include "txComponentHeader.h"
#include "GameLog.h"
#include "Utility.h"

txComponent::txComponent(const std::string& typeName, const std::string& name)
:
mComponentOwner(NULL),
mBaseType(""),
mType(typeName),
mName(name),
mActive(true),
mLockOneFrame(false),
mParent(NULL),
mNeedPreUpdate(false)
{}

void txComponent::destroy()
{
	// 首先通知所有的子组件
	int childCount = mChildComponentList.size();
	FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
	{
		mChildComponentList[i]->notifyParentDestroied();
	}
	END_FOR_STL(mChildComponentList);
	mChildComponentList.clear();
	mChildComponentMap.clear();

	// 通知父组件
	if (mParent != NULL)
	{
		mParent->notifyChildDestroied(this);
		mParent = NULL;
	}

	if (mComponentOwner != NULL)
	{
		mComponentOwner->notifyComponentDestroied(this);
	}
}

void txComponent::preUpdate(float elapsedTime)
{
	if (mLockOneFrame || !isActive())
	{
		return;
	}
	// 预更新子组件
	int childCount = mChildComponentList.size();
	FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
	{
		mChildComponentList[i]->preUpdate(elapsedTime);
	}
	END_FOR_STL(mChildComponentList);
}

void txComponent::update(float elapsedTime)
{
	if (mLockOneFrame || !isActive())
	{
		return;
	}
	// 更新子组件
	int childCount = mChildComponentList.size();
	for (int i = 0; i < childCount; ++i)
	{
		mChildComponentList[i]->update(elapsedTime);
	}
}

void txComponent::lateUpdate(float elapsedTime)
{
	if (mLockOneFrame)
	{
		mLockOneFrame = false;
		return;
	}
	if (!isActive())
	{
		return;
	}
	// 后更新子组件
	int childCount = mChildComponentList.size();
	FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
	{
		mChildComponentList[i]->lateUpdate(elapsedTime);
	}
	END_FOR_STL(mChildComponentList);
}

bool txComponent::rename(const std::string& newName)
{
	if (mName == newName)
	{
		return false;
	}
	std::string oldName = mName;
	mName = newName;
	if (mComponentOwner != NULL)
	{
		// 通知Layout自己的名字改变了
		bool ret = mComponentOwner->notifyComponentNameChanged(oldName, this);
		// 如果父窗口不允许自己改名,则需要将名字还原
		if (!ret)
		{
			mName = oldName;
			return false;
		}
	}
	// 通知父窗口自己的名字改变了
	if (NULL != mParent)
	{
		mParent->notifyChildNameChanged(oldName, this);
	}
	return true;
}

bool txComponent::addChild(txComponent* component)
{
	if (component == NULL || mChildComponentMap.find(component->getName()) != mChildComponentMap.end())
	{
		return false;
	}
	mChildComponentList.push_back(component);
	mChildComponentMap.insert(component->getName(), component);
	return true;
}

bool txComponent::removeChild(txComponent* component)
{
	if (component == NULL)
	{
		return false;
	}
	txMap<std::string, txComponent*>::iterator iter = mChildComponentMap.find(component->getName());
	if (iter == mChildComponentMap.end())
	{
		return false;
	}
	mChildComponentMap.erase(iter);
	txVector<txComponent*>::iterator iterList = mChildComponentList.begin();
	txVector<txComponent*>::iterator iterListEnd = mChildComponentList.end();
	FOR_STL(mChildComponentList, ; iterList != iterListEnd; ++iterList)
	{
		if (*iterList == component)
		{
			mChildComponentList.erase(iterList);
			break;
		}
	}
	END_FOR_STL(mChildComponentList);
	return true;
}

bool txComponent::isActive()
{
	if (mParent != NULL)
	{
		return mParent->isActive() && mActive;
	}
	return mActive;
}

void txComponent::detachOwnerParentComponent(bool detachOwnerOnly)
{
	if (mComponentOwner != NULL)
	{
		mComponentOwner->notifyComponentDetached(this);
		mComponentOwner = NULL;
	}
	// 如果不是只断开与布局的联系,则还需要断开与父窗口的联系
	if (!detachOwnerOnly && mParent != NULL)
	{
		mParent->notifyChildDetached(this);
		mParent = NULL;
	}
	// 使自己所有的子窗口都断开与布局的联系,但是不能打断子窗口的父子关系
	int childCount = mChildComponentList.size();
	FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
	{
		mChildComponentList[i]->detachOwnerParentComponent(true);
	}
	END_FOR_STL(mChildComponentList);
}
// 建立与布局和父窗口的联系,使该窗口成为布局中的一个窗口,该窗口下的所有子窗口也会重建与布局的联系,父子关系仍然存在
void txComponent::attachOwnerParentComponent(txComponentOwner* owner, txComponent* parent, int childPos)
{
	if (owner != NULL && mComponentOwner == NULL)
	{
		mComponentOwner = owner;
		mComponentOwner->notifyComponentAttached(this);
		// 使自己所有的子窗口都建立与布局的联系
		int childCount = mChildComponentList.size();
		FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
		{
			mChildComponentList[i]->attachOwnerParentComponent(owner, NULL, -1);
		}
		END_FOR_STL(mChildComponentList);
	}
	if (parent != NULL && mParent == NULL)
	{
		parent->addChild(this);
		parent->moveChildPos(this, childPos);
	}
}

int txComponent::getChildPos(txComponent* window)
{
	if (window == NULL)
	{
		return -1;
	}
	// 首先查找当前窗口的位置
	int index = -1;
	int childCount = mChildComponentList.size();
	FOR_STL(mChildComponentList, int i = 0; i < childCount; ++i)
	{
		if (window == mChildComponentList[i])
		{
			index = i;
			break;
		}
	}
	END_FOR_STL(mChildComponentList);
	return index;
}

bool txComponent::moveChildPos(txComponent* component, int destPos)
{
	if (component == NULL || destPos < 0 || destPos >= (int)mChildComponentList.size())
	{
		return false;
	}
	// 首先查找当前窗口的位置
	int pos = getChildPos(component);
	if (pos < 0 || pos == destPos)
	{
		return false;
	}
	mChildComponentList.erase(mChildComponentList.begin() + pos);
	mChildComponentList.insert(mChildComponentList.begin() + destPos, component);
	return true;
}

bool txComponent::moveChildPos(const std::string& name, int destPos)
{
	return moveChildPos(getChildComponent(name), destPos);
}

void txComponent::notifyChildNameChanged(const std::string& oldName, txComponent* component)
{
	// 修改全部子窗口查找列表中的名字
	txMap<std::string, txComponent*>::iterator iterAll = mChildComponentMap.find(oldName);
	if (iterAll != mChildComponentMap.end())
	{
		txMap<std::string, txComponent*>::iterator iterNew = mChildComponentMap.find(component->mName);
		if (iterNew == mChildComponentMap.end())
		{
			mChildComponentMap.insert(component->mName, component);
			mChildComponentMap.erase(iterAll);
		}
		else
		{
			LOG_ERROR("error : there is a child named : %s!", component->mName.c_str());
		}
	}
}
