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
	T* newCmd(const bool& show = true, const bool& delay = false)
	{
		LOCK(mNewCmdLock); 
		// ���ȴ�δʹ�õ��б��л�ȡ,��ȡ���������´���һ��
		T* cmd = NULL;
		std::string type = typeid(T).name();
		auto iterUnuse = mUnusedList.find(type);
		if (iterUnuse != mUnusedList.end() && iterUnuse->second.size() > 0)
		{
			cmd = (T*)iterUnuse->second[0];
			// ��δʹ���б����Ƴ�
			removeUnuse(cmd);
		}
		// û���ҵ������õ�,�򴴽�һ��
		if (cmd == NULL)
		{
			cmd = TRACE_NEW(T, cmd);
			cmd->setID(mIDSeed++);
			cmd->init();
			cmd->setType(type);
			++mNewCount;
			LOG_INFO("new cmd : %d, type : %s", mNewCount, type.c_str());
		}
		// ����Ϊ��������
		cmd->setValid(true);
		cmd->setAssignID(delay ? (mAssignIDSeed++) : -1);
		cmd->setShowDebugInfo(show);
		cmd->setDelayCommand(delay);
		// ������ʹ���б�
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
	ThreadLock mNewCmdLock;	// ֻ��Ҫ��Ӵ�����������Ϳ���,ֻҪ��������ظ�������,��������ʱ�Ͳ��ᷢ����ͻ
	int mNewCount;
	static int mIDSeed;
	static int mAssignIDSeed;
};

#endif