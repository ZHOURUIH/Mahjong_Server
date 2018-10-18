#ifndef _COMMAND_POOL_H_
#define _COMMAND_POOL_H_

#include "ServerDefine.h"
#include "ServerBase.h"
#include "txCommand.h"
#include "ThreadLock.h"
#include "Utility.h"
#include "GameLog.h"

class CommandPool : ServerBase
{
public:
	CommandPool()
	{
		mNewCount = 0;
	}
	void init()
	{
		;
	}
	void destroy();
	template<typename T>
	T* newCmd(bool show = true, bool delay = false)
	{
		T* cmd = NULL;
		LOCK(mNewCmdLock); 
		// 首先从未使用的列表中获取,获取不到再重新创建一个
		std::string type = typeid(T).name();
		auto iterUnuse = mUnusedList.find(type);
		if (iterUnuse != mUnusedList.end() && iterUnuse->second.size() > 0)
		{
			cmd = (T*)iterUnuse->second[0];
			// 从未使用列表中移除
			removeUnuse(cmd);
		}
		// 没有找到可以用的,则创建一个
		if (cmd == NULL)
		{
			TRACE_NEW(T, cmd);
			cmd->setID(mIDSeed++);
			cmd->init();
			cmd->setType(type);
			++mNewCount;
		}
		// 设置为可用命令
		cmd->setValid(true);
		cmd->setAssignID(delay ? (mAssignIDSeed++) : -1);
		cmd->setShowDebugInfo(show);
		cmd->setDelayCommand(delay);
		// 加入已使用列表
		addInuse(cmd);
		UNLOCK(mNewCmdLock);
		return cmd;
	}
	void destroyCmd(txCommand* cmd);
	//------------------------------------------------------------------------------------------------------------------
protected:
	void addInuse(txCommand* cmd);
	void addUnuse(txCommand* cmd);
	void removeInuse(txCommand* cmd);
	void removeUnuse(txCommand* cmd);
protected:
	txMap<std::string, txVector<txCommand*>> mInusedList;
	txMap<std::string, txVector<txCommand*>> mUnusedList;
	ThreadLock mInuseLock;
	ThreadLock mUnuseLock;
	ThreadLock mNewCmdLock;	// 只需要添加创建命令的锁就可以,只要不分配出重复的命令,回收命令时就不会发生冲突
	int mNewCount;
	static int mIDSeed;
	static int mAssignIDSeed;
};

#endif