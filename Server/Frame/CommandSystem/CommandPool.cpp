﻿#include "CommandPool.h"

int CommandPool::mIDSeed = 0;
int CommandPool::mAssignIDSeed = 0;

void CommandPool::destroy()
{
	auto iterUse = mInusedList.begin();
	auto iterUseEnd = mInusedList.end();
	FOR_STL(mInusedList, ; iterUse != iterUseEnd; ++iterUse)
	{
		int count = iterUse->second.size();
		FOR_STL(iterUse->second, int i = 0; i < count; ++i)
		{
			TRACE_DELETE(iterUse->second[i]);
		}
		END_FOR_STL(iterUse->second);
		iterUse->second.clear();
	}
	END_FOR_STL(mInusedList);

	auto iterUnuse = mUnusedList.begin();
	auto iterUnuseEnd = mUnusedList.end();
	FOR_STL(mUnusedList, ; iterUnuse != iterUnuseEnd; ++iterUnuse)
	{
		int count = iterUnuse->second.size();
		FOR_STL(iterUnuse->second, int i = 0; i < count; ++i)
		{
			TRACE_DELETE(iterUnuse->second[i]);
		}
		END_FOR_STL(iterUnuse->second);
		iterUnuse->second.clear();
	}
	END_FOR_STL(mUnusedList);
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