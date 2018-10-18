#include "CommandPool.h"

int CommandPool::mIDSeed = 0;
int CommandPool::mAssignIDSeed = 0;

void CommandPool::destroy()
{
	auto iterUse = mInusedList.begin();
	auto iterUseEnd = mInusedList.end();
	FOR(mInusedList, ; iterUse != iterUseEnd; ++iterUse)
	{
		auto& list = iterUse->second;
		int count = list.size();
		FOR(list, int i = 0; i < count; ++i)
		{
			TRACE_DELETE(list[i]);
		}
		END(list);
		list.clear();
	}
	END(mInusedList);

	auto iterUnuse = mUnusedList.begin();
	auto iterUnuseEnd = mUnusedList.end();
	FOR(mUnusedList, ; iterUnuse != iterUnuseEnd; ++iterUnuse)
	{
		auto& list = iterUnuse->second;
		int count = list.size();
		FOR(list, int i = 0; i < count; ++i)
		{
			TRACE_DELETE(list[i]);
		}
		END(list);
		list.clear();
	}
	END(mUnusedList);
}

void CommandPool::destroyCmd(txCommand* cmd)
{
	// 销毁命令时,初始化命令数据,并设置为不可用命令
	cmd->init();
	addUnuse(cmd);
	removeInuse(cmd);
}

void CommandPool::addInuse(txCommand* cmd)
{
	LOCK(mInuseLock);
	// 添加到使用列表中
	auto iterInuse = mInusedList.tryInsert(cmd->getType(), txVector<txCommand*>());
	iterInuse->second.push_back(cmd);
	UNLOCK(mInuseLock);
}
void CommandPool::addUnuse(txCommand* cmd)
{
	LOCK(mUnuseLock);
	// 添加到未使用列表中
	auto iterUnuse = mUnusedList.tryInsert(cmd->getType(), txVector<txCommand*>());
	iterUnuse->second.push_back(cmd);
	UNLOCK(mUnuseLock);
}
void CommandPool::removeInuse(txCommand* cmd)
{
	LOCK(mInuseLock);
	mInusedList.tryErase(cmd->getType());
	UNLOCK(mInuseLock);
}
void CommandPool::removeUnuse(txCommand* cmd)
{
	LOCK(mUnuseLock);
	mUnusedList.tryErase(cmd->getType());
	UNLOCK(mUnuseLock);
}