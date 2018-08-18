#ifndef _TX_COMPONENT_H_
#define _TX_COMPONENT_H_

#include "ServerDefine.h"

class txComponentOwner;
class txComponent
{
public:
	txComponent(const std::string& typeName, const std::string& name);
	virtual ~txComponent(){ destroy(); }
	virtual void init(txComponentOwner* owner)
	{
		mComponentOwner = owner;
		// setBaseType不能在构造中调用,因为构造时对象还没有完全创建出,所以无法正确调用派生类的函数
		setBaseType();
	}
	virtual void setBaseType() = 0;
	virtual void preUpdate(float elapsedTime);
	virtual void update(float elapsedTime);
	virtual void lateUpdate(float elapsedTime);
	void destroy();
	bool rename(const std::string& newName);
	bool addChild(txComponent* component);		// 将一个组件添加作为当前组件的子组件
	bool removeChild(txComponent* component);	// 将一个组件从当前组件的子组件列表中移除
	bool isActive();
	// 断开与拥有者和父组件的联系,使该组件成为一个独立的组件,该组件下的所有子组件也会断开与拥有者的联系,但是父子关系仍然存在
	// detachOwnerOnly表示是否只断开与拥有者的联系,不断开组件之间的父子关系,外部使用时一般填false
	virtual void detachOwnerParentComponent(bool detachOwnerOnly = false);
	// 建立与拥有者和父组件的联系,使该组件成为拥有者中的一个组件,该组件下的所有子组件也会重建与拥有者的联系,父子关系仍然存在
	virtual void attachOwnerParentComponent(txComponentOwner* owner, txComponent* parent, int childPos);
	int getChildPos(txComponent* component);					// 得到指定组件在当前组件中的位置
	bool moveChildPos(txComponent* component, int destPos);		// 将指定组件移动到当前组件中的指定位置
	bool moveChildPos(const std::string& name, int destPos);
	txComponent* getChildComponent(const std::string& childName)
	{
		auto iter = mChildComponentMap.find(childName);
		if (iter != mChildComponentMap.end())
		{
			return iter->second;
		}
		return NULL;
	}

	// 设置成员变量
	virtual void setActive(bool active)						{ mActive = active; }
	void setLockOneFrame(bool lock)							{ mLockOneFrame = lock; }
	virtual void setParentComponent(txComponent* component)	{ mParent = component; }

	// 获得成员变量
	txComponentOwner* getOwner()		{ return mComponentOwner; }
	const bool& isNeedPreUpdate()		{ return mNeedPreUpdate; }
	txComponent* getParentComponent()	{ return mParent; }
	const std::string& getType()		{ return mType; }
	const std::string& getBaseType()	{ return mBaseType; }
	const std::string& getName()		{ return mName; }
	const bool& isLockOneFrame()		{ return mLockOneFrame; }
	const bool& isComponentActive()		{ return mActive; }		// 组件自己是否激活,不考虑父组件
	const txVector<txComponent*>& getChildComponentList() { return mChildComponentList; }
	const txMap<std::string, txComponent*>& getChildComponentMap() { return mChildComponentMap; }

	// 通知
	virtual void notifyParentDestroied(){ mParent = NULL; }
	virtual void notifyChildDestroied(txComponent* component){ removeChild(component); }
	virtual void notifyChildDetached(txComponent* component){ removeChild(component); }	// 通知该组件有子组件断开了联系
	virtual void notifyChildNameChanged(const std::string& oldName, txComponent* component);
protected:
	// 只是为子类提供一个公用的函数
	template<typename FUNC, typename CLASS>
	static void setCallback(FUNC callback, void* userData, FUNC& curCallback, void*& curUserData, CLASS* component)
	{
		FUNC tempCallback = curCallback;
		void* tempUserData = curUserData;
		curCallback = NULL;
		curUserData = NULL;
		// 如果回调函数当前不为空,则是中断了正在进行的变化
		if (tempCallback != NULL)
		{
			tempCallback(component, tempUserData, true);
		}
		curCallback = callback;
		curUserData = userData;
	}
	template<typename FUNC, typename CLASS>
	void doneCallback(FUNC& curDoneCallback, void*& curDoneUserData, CLASS* component)
	{
		// 先保存回调,然后再调用回调之前就清空回调,确保在回调函数执行时已经完全完成
		FUNC tempCallback = curDoneCallback;
		void* tempUserData = curDoneUserData;
		clearCallback();
		if (tempCallback != NULL)
		{
			tempCallback(component, tempUserData, false);
		}
	}
	virtual void clearCallback(){}
protected:
	txComponentOwner* mComponentOwner;	// 该组件的拥有者
	std::string mBaseType;				// 基础组件类型,指定该组件属于什么基础类型的组件,如果不属于任何基础类型,则填实际组件类型
	std::string mType;					// 实际组件类型
	std::string mName;					// 组件名
	bool mActive;						// 是否激活组件
	bool mLockOneFrame;					// 是否将组件锁定一次
	txComponent* mParent;				// 父级组件
	txMap<std::string, txComponent*> mChildComponentMap;	// 用于查找组件
	txVector<txComponent*> mChildComponentList;			// 该组件下的子组件列表,保存了子组件之间的顺序
	bool mNeedPreUpdate;				// 是否为需要预先更新的组件
};

#endif