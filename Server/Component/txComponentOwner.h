#ifndef _TX_COMPONENT_OWNER_H_
#define _TX_COMPONENT_OWNER_H_

#include "ServerDefine.h"
#include "txCommandReceiver.h"
#include "ServerBase.h"

class txProperty;
class txComponent;
class txComponentOwner : public txCommandReceiver, public ServerBase
{
public:
	txComponentOwner(const std::string& name)
		:
		txCommandReceiver(name)
	{}
	virtual ~txComponentOwner(){ destroyAllComponents(); }
	virtual void initComponents() = 0;
	virtual void updatePreComponent(const float& elapsedTime);		// 更新需要最先更新的组件
	virtual void updateComponents(const float& elapsedTime);		// 更新正常更新的组件
	virtual void notifyAddComponent(txComponent* component){}
	// 通知布局窗口断开了与布局的联系,由窗口发出
	virtual void notifyComponentDetached(txComponent* component){ removeComponentFromList(component); }
	// 通知布局窗口建立了与布局的联系,由窗口发出
	virtual void notifyComponentAttached(txComponent* component);
	virtual bool notifyComponentNameChanged(const std::string& oldName, txComponent* component);
	static txComponent* createIndependentComponent(const std::string& name, const std::string& type, const bool& initComponent = true);
	txComponent* addComponent(const std::string& name, const std::string& type);
	static void destroyComponent(txComponent* component);
	void destroyComponent(const std::string& name);
	void destroyAllComponents();
	txComponent* getComponent(const std::string& name)
	{
		std::map<std::string, txComponent*>::iterator itrFind = mAllComponentList.find(name);
		if (itrFind == mAllComponentList.end())
		{
			return NULL;
		}
		return itrFind->second;
	}
	const std::map<std::string, txComponent*>& getComponentsByType(const std::string& type)
	{
		std::map<std::string, std::map<std::string, txComponent*> >::iterator iterType = mAllComponentTypeList.find(type);
		if (iterType != mAllComponentTypeList.end())
		{
			return iterType->second;
		}
		return EMPTY_COMPONENT_MAP;
	}
	const std::map<std::string, txComponent*>& getComponentsByBaseType(const std::string& type)
	{
		std::map<std::string, std::map<std::string, txComponent*> >::iterator iterType = mAllComponentBaseTypeList.find(type);
		if (iterType != mAllComponentBaseTypeList.end())
		{
			return iterType->second;
		}
		return EMPTY_COMPONENT_MAP;
	}
	txComponent* getFirstActiveComponentByBaseType(const std::string& type);
	txComponent* getFirstComponentByBaseType(const std::string& type)
	{
		std::map<std::string, std::map<std::string, txComponent*> >::iterator iter = mAllComponentBaseTypeList.find(type);
		if (iter != mAllComponentBaseTypeList.end() && iter->second.size() > 0)
		{
			return iter->second.begin()->second;
		}
		return NULL;
	}
	txComponent* getFirstActiveComponent(const std::string& type);
	txComponent* getFirstComponent(const std::string& type)
	{
		std::map<std::string, std::map<std::string, txComponent*> >::iterator iter = mAllComponentTypeList.find(type);
		if (iter != mAllComponentTypeList.end() && iter->second.size() > 0)
		{
			return iter->second.begin()->second;
		}
		return NULL;
	}
	void addFirstUpdateType(const std::string& typeName)
	{
		mPreUpdateTypeList.insert(typeName);
	}
	void removePreUpdateType(const std::string& typeName)
	{
		std::set<std::string>::iterator iter = mPreUpdateTypeList.find(typeName);
		if (iter != mPreUpdateTypeList.end())
		{
			mPreUpdateTypeList.erase(iter);
		}
	}
	bool isPreUpdateType(const std::string& type)
	{
		return mPreUpdateTypeList.find(type) != mPreUpdateTypeList.end();
	}
	const std::map<std::string, std::map<std::string, txComponent*> >& getComponentTypeList() { return mAllComponentTypeList; }
	virtual void notifyComponentDestroied(txComponent* component)
	{
		removeComponentFromList(component);
	}
	const std::vector<txComponent*>& getRootComponentList() { return mRootComponentList; }
protected:
	void addComponentToList(txComponent* component, const int& componentPos = -1);
	void removeComponentFromList(txComponent* component);
protected:
	std::vector<txComponent*> mRootComponentList;				// 一级组件列表,保存着组件之间的更新顺序
	std::map<std::string, txComponent*> mAllComponentList;	// 组件拥有者当前的所有组件列表
	std::map<std::string, std::map<std::string, txComponent*> > mAllComponentTypeList;	// 组件类型列表,first时组件的类型名
	// 根据组件的基础类型分组的组件列表,first是基础组件类型名
	std::map<std::string, std::map<std::string, txComponent*> > mAllComponentBaseTypeList;
	std::set<std::string> mPreUpdateTypeList;	// 需要提前更新的组件类型名
	static std::map<std::string, txComponent*> EMPTY_COMPONENT_MAP;
};

#endif