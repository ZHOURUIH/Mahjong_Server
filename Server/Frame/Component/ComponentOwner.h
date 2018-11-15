#ifndef _COMPONENT_OWNER_H_
#define _COMPONENT_OWNER_H_

#include "ServerDefine.h"
#include "CommandReceiver.h"

class txProperty;
class txComponent;
class ComponentOwner : public CommandReceiver
{
public:
	ComponentOwner(const string& name)
		:
		CommandReceiver(name),
		mUsePreLateUpdate(false)
	{}
	virtual ~ComponentOwner(){ destroyAllComponents(); }
	virtual void initComponents() = 0;
	virtual void updatePreComponent(float elapsedTime);		// 更新需要最先更新的组件
	virtual void updateComponents(float elapsedTime);		// 更新正常更新的组件
	virtual void notifyAddComponent(txComponent* component){}
	// 通知布局窗口断开了与布局的联系,由窗口发出
	virtual void notifyComponentDetached(txComponent* component){ removeComponentFromList(component); }
	// 通知布局窗口建立了与布局的联系,由窗口发出
	virtual void notifyComponentAttached(txComponent* component);
	virtual bool notifyComponentNameChanged(const string& oldName, txComponent* component);
	static txComponent* createIndependentComponent(const string& name, const string& type, bool initComponent = true);
	txComponent* addComponent(const string& name, const string& type);
	static void destroyComponent(txComponent* component);
	void destroyComponent(const string& name);
	void destroyAllComponents();
	txComponent* getComponent(const string& name)
	{
		return mAllComponentList.tryGet(name, NULL);
	}
	const txMap<string, txComponent*>& getComponentsByType(const string& type)
	{
		return mAllComponentTypeList.tryGet(type, EMPTY_COMPONENT_MAP);
	}
	const txMap<string, txComponent*>& getComponentsByBaseType(const string& type)
	{
		return mAllComponentBaseTypeList.tryGet(type, EMPTY_COMPONENT_MAP);
	}
	txComponent* getFirstActiveComponentByBaseType(const string& type);
	txComponent* getFirstComponentByBaseType(const string& type)
	{
		auto iter = mAllComponentBaseTypeList.find(type);
		if (iter != mAllComponentBaseTypeList.end() && iter->second.size() > 0)
		{
			return iter->second.begin()->second;
		}
		return NULL;
	}
	txComponent* getFirstActiveComponent(const string& type);
	txComponent* getFirstComponent(const string& type)
	{
		auto iter = mAllComponentTypeList.find(type);
		if (iter != mAllComponentTypeList.end() && iter->second.size() > 0)
		{
			return iter->second.begin()->second;
		}
		return NULL;
	}
	void addFirstUpdateType(const string& typeName)
	{
		mPreUpdateTypeList.insert(typeName);
	}
	void removePreUpdateType(const string& typeName)
	{
		mPreUpdateTypeList.tryErase(typeName);
	}
	bool isPreUpdateType(const string& type)
	{
		return mPreUpdateTypeList.contains(type);
	}
	const txMap<string, txMap<string, txComponent*> >& getComponentTypeList() { return mAllComponentTypeList; }
	virtual void notifyComponentDestroied(txComponent* component)
	{
		removeComponentFromList(component);
	}
	const txVector<txComponent*>& getRootComponentList() { return mRootComponentList; }
protected:
	void addComponentToList(txComponent* component, int componentPos = -1);
	void removeComponentFromList(txComponent* component);
protected:
	txVector<txComponent*> mRootComponentList;				// 一级组件列表,保存着组件之间的更新顺序
	txMap<string, txComponent*> mAllComponentList;	// 组件拥有者当前的所有组件列表
	txMap<string, txMap<string, txComponent*> > mAllComponentTypeList;	// 组件类型列表,first时组件的类型名
	// 根据组件的基础类型分组的组件列表,first是基础组件类型名
	txMap<string, txMap<string, txComponent*> > mAllComponentBaseTypeList;
	txSet<string> mPreUpdateTypeList;	// 需要提前更新的组件类型名
	static txMap<string, txComponent*> EMPTY_COMPONENT_MAP;
	bool mUsePreLateUpdate;
};

#endif