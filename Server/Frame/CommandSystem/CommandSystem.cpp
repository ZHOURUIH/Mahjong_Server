#include "Utility.h"
#include "CommandSystem.h"
#include "CommandReceiver.h"
#include "txCommand.h"
#include "GameLog.h"
#include "MemoryDefine.h"

CommandSystem::CommandSystem(const string& name)
	:FrameComponent(name)
{
	TRACE_NEW(CommandPool, mCommandPool);
	mSystemDestroy = false;
}

void CommandSystem::update(float elapsedTime)
{
	// 同步命令输入列表到命令处理列表中
	syncCommandBuffer();

	// 如果一帧时间大于1秒,则认为是无效更新
	if (elapsedTime >= 1.0f)
	{
		return;
	}
	// 执行之前需要先清空列表
	mExecuteList.clear();
	// 开始处理命令处理列表
	for (int i = 0; i < mCommandBufferProcess.size(); ++i)
	{
		mCommandBufferProcess[i]->mDelayTime -= elapsedTime;
		if (mCommandBufferProcess[i]->mDelayTime <= 0.0f)
		{
			// 命令的延迟执行时间到了,则执行命令
			mExecuteList.push_back(mCommandBufferProcess[i]);
			mCommandBufferProcess.erase(mCommandBufferProcess.begin() + i);
			--i;
		}
	}
	int executeCount = mExecuteList.size();
	for (int i = 0; i < executeCount; ++i)
	{
		mExecuteList[i]->mCommand->setDelayCommand(false);
		if (mExecuteList[i]->mReceiver != NULL)
		{
			pushCommand(mExecuteList[i]->mCommand, mExecuteList[i]->mReceiver);
		}
		TRACE_DELETE(mExecuteList[i]);
	}
	// 执行完后清空列表
	mExecuteList.clear();
}

bool CommandSystem::interruptCommand(int assignID)
{
	// 如果命令系统已经销毁了,则不能再中断命令
	if (mSystemDestroy)
	{
		return true;
	}
	if (assignID < 0)
	{
		LOG_ERROR("assignID invalid! : " + intToString(assignID));
		return false;
	}
	syncCommandBuffer();
	bool ret = false;
	int count = mCommandBufferProcess.size();
	FOR(mCommandBufferProcess, int i = 0; i < count; ++i)
	{
		DelayCommand* delayInfo = mCommandBufferProcess[i];
		if (delayInfo->mCommand->getAssignID() == assignID)
		{
			LOG_INFO("CommandSystem : interrupt command : " + intToString(assignID) + ", " + delayInfo->mCommand->showDebugInfo() + ", receiver : " + delayInfo->mReceiver->getName());
			// 销毁回收命令
			mCommandPool->destroyCmd(delayInfo->mCommand);
			mCommandBufferProcess.erase(mCommandBufferProcess.begin() + i);
			TRACE_DELETE(delayInfo);
			ret = true;
			break;
		}
	}
	END(mCommandBufferProcess);
	// 在即将执行的列表中查找
	if (!ret)
	{
		int count0 = mExecuteList.size();
		FOR(mExecuteList, int i = 0; i < count0; ++i)
		{
			if (mExecuteList[i]->mCommand->getAssignID() == assignID)
			{
				LOG_ERROR("cmd is in execute list! can not interrupt!");
				break;
			}
		}
		END(mExecuteList);
	}
	LOG_ERROR("not find cmd with assignID! " + intToString(assignID));
	return ret;
}

void CommandSystem::pushCommand(txCommand* cmd, CommandReceiver* cmdReceiver)
{
	if (cmd == NULL)
	{
		string receiverName = (cmdReceiver != NULL) ? cmdReceiver->getName() : EMPTY_STRING;
		LOG_ERROR("cmd is null! receiver : " + receiverName);
		return;
	}
	if (cmdReceiver == NULL)
	{
		string cmdName = (cmd != NULL) ? cmd->getType() : EMPTY_STRING;
		LOG_ERROR("receiver is null! cmd : " + cmdName);
		return;
	}
	if (!cmd->isValid())
	{
		LOG_ERROR("cmd is invalid! make sure create cmd use CommandSystem.newCmd! pushCommand cmd type : " + cmd->getType() + ", assign id : " + intToString(cmd->getAssignID()));
		return;
	}
	if (cmd->isDelayCommand())
	{
		LOG_ERROR("cmd is a delay cmd! can not use pushCommand! assign id : " + intToString(cmd->getAssignID()) + ", info : " + cmd->showDebugInfo());
		return;
	}
	cmd->setReceiver(cmdReceiver);
	if (cmd->getShowDebugInfo())
	{
		LOG_INFO("CommandSystem : " + intToString(cmd->getAssignID()) + ", " + cmd->showDebugInfo() + ", receiver : " + cmdReceiver->getName());
	}
	cmdReceiver->receiveCommand(cmd);

	// 销毁回收命令
	mCommandPool->destroyCmd(cmd);
}

void CommandSystem::pushDelayCommand(txCommand* cmd, CommandReceiver* cmdReceiver, float delayExecute)
{
	if (cmd == NULL)
	{
		string receiverName = (cmdReceiver != NULL) ? cmdReceiver->getName() : EMPTY_STRING;
		LOG_ERROR("cmd is null! receiver : " + receiverName);
		return;
	}
	if (cmdReceiver == NULL)
	{
		string cmdName = (cmd != NULL) ? cmd->getType() : EMPTY_STRING;
		LOG_ERROR("receiver is null! cmd : " + cmdName);
		return;
	}
	if (!cmd->isValid())
	{
		LOG_ERROR("cmd is invalid! make sure create cmd use CommandSystem.newCmd! pushDelayCommand cmd type : " + cmd->getType() + ", assign id : " + intToString(cmd->getAssignID()));
		return;
	}
	if (!cmd->isDelayCommand())
	{
		LOG_ERROR("cmd is not a delay command, Command : " + intToString(cmd->getAssignID()) + ", info : " + cmd->showDebugInfo());
		return;
	}
	if (delayExecute < 0.0f)
	{
		delayExecute = 0.0f;
	}
	if (cmd->getShowDebugInfo())
	{
		LOG_INFO("CommandSystem : delay cmd : " + intToString(cmd->getAssignID()) + ", " + floatToString(delayExecute, 2) + ", info : " + cmd->showDebugInfo() + ", receiver : " + cmdReceiver->getName());
	}
	DelayCommand* delayCommand = TRACE_NEW(DelayCommand, delayCommand, delayExecute, cmd, cmdReceiver);

	LOCK(mBufferLock);
	mCommandBufferInput.push_back(delayCommand);
	UNLOCK(mBufferLock);
}

void CommandSystem::destroy()
{
	syncCommandBuffer();
	int processSize = mCommandBufferProcess.size();
	FOR(mCommandBufferProcess, int i = 0; i < processSize; ++i)
	{
		mCommandPool->destroyCmd(mCommandBufferProcess[i]->mCommand);
		TRACE_DELETE(mCommandBufferProcess[i]);
	}
	END(mCommandBufferProcess);
	mCommandBufferProcess.clear();

	mCommandPool->destroy();
	mSystemDestroy = true;
}

void CommandSystem::notifyReceiverDestroied(CommandReceiver* receiver)
{
	if (mSystemDestroy)
	{
		return;
	}
	// 先将命令
	syncCommandBuffer();
	// 同步列表中
	FOR (mCommandBufferProcess, int i = 0; i < mCommandBufferProcess.size(); ++i)
	{
		if (mCommandBufferProcess[i]->mReceiver == receiver)
		{
			mCommandPool->destroyCmd(mCommandBufferProcess[i]->mCommand);
			TRACE_DELETE(mCommandBufferProcess[i]);
			mCommandBufferProcess.erase(mCommandBufferProcess.begin() + i, false);
			--i;
		}
	}
	END(mCommandBufferProcess);
	// 执行列表中
	int count = mExecuteList.size();
	FOR (mExecuteList, int i = 0; i < count; ++i)
	{
		// 已执行或正在执行的命令不作判断,该列表无法删除元素,只能将接收者设置为null
		if (mExecuteList[i] != NULL && mExecuteList[i]->mReceiver == receiver && mExecuteList[i]->mCommand->getExecuteState() == ES_NOT_EXECUTE)
		{
			mExecuteList[i]->mReceiver = NULL;
		}
	}
	END(mExecuteList);
}

void CommandSystem::syncCommandBuffer()
{
	LOCK(mBufferLock);
	int inputCount = mCommandBufferInput.size();
	if (inputCount > 0)
	{
		FOR (mCommandBufferInput, int i = 0; i < inputCount; ++i)
		{
			mCommandBufferProcess.push_back(mCommandBufferInput[i]);
		}
		END(mCommandBufferInput);
		mCommandBufferInput.clear();
	}
	UNLOCK(mBufferLock);
}