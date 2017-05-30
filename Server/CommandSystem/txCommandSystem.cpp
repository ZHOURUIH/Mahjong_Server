#include "txUtility.h"
#include "txCommandSystem.h"
#include "txCommandReceiver.h"
#include "txCommand.h"
#include "GameLog.h"

txCommandSystem::~txCommandSystem()
{
	destroy();
}

void txCommandSystem::init(const bool& showDebug)
{
	mShowDebugInfo = showDebug;
}

void txCommandSystem::update(const float& elapsedTime)
{
	// 同步命令输入列表到命令处理列表中
	// 等待解锁缓冲区,锁定缓冲区
	LOCK(mBufferLock, LT_WRITE);
	int inputCount = mCommandBufferInput.size();
	for (int i = 0; i < inputCount; ++i)
	{
		mCommandBufferProcess.push_back(mCommandBufferInput[i]);
	}
	mCommandBufferInput.clear();
	// 解锁缓冲区
	UNLOCK(mBufferLock, LT_WRITE);

	// 如果一帧时间大于1秒,则认为是无效更新
	if (elapsedTime >= 1.0f)
	{
		return;
	}
	// 开始处理命令处理列表
	std::vector<DelayCommand>::iterator iter = mCommandBufferProcess.begin();
	for (; iter != mCommandBufferProcess.end();)
	{
		iter->mDelayTime -= elapsedTime;
		if (iter->mDelayTime <= 0.0f)
		{
			// 命令的延迟执行时间到了,则执行命令
			iter->mCommand->setDelayCommand(false);
			pushCommand(iter->mCommand, iter->mReceiver);
			TRACE_DELETE(iter->mCommand);
			iter = mCommandBufferProcess.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void txCommandSystem::pushCommand(txCommand* cmd, txCommandReceiver* cmdReceiver)
{
	if (cmd == NULL || cmdReceiver == NULL)
	{
		return;
	}
	if (cmd->isDelayCommand())
	{
		GAME_ERROR("error : cmd is a delay command, can not use pushCommand!");
	}
	cmd->setReceiver(cmdReceiver);
	if (mShowDebugInfo && cmd->getShowDebugInfo())
	{
		COMMAND_INFO("%s | CommandSystem : 0x%p, %s, receiver : %s, file : %s, line : %d", txUtility::getTime(), 
			cmd, cmd->showDebugInfo().c_str(), cmdReceiver->getName().c_str(), txUtility::getFileName(cmd->getFile()).c_str(), cmd->getLine());
	}
	cmdReceiver->receiveCommand(cmd);
}

void txCommandSystem::pushDelayCommand(txCommand* cmd, txCommandReceiver* cmdReceiver, const float& delayExecute)
{
	if (cmd == NULL || cmdReceiver == NULL)
	{
		return;
	}
	if (delayExecute < 0.0f)
	{
		GAME_ERROR("error : delay command time can not less than 0");
		return;
	}
	if (mShowDebugInfo && cmd->getShowDebugInfo())
	{
		COMMAND_INFO("%s | CommandSystem : delay cmd : %f, info : 0x%p, %s, receiver : %s, file : %s, line : %d", txUtility::getTime(),
			delayExecute, cmd, cmd->showDebugInfo().c_str(), cmdReceiver->getName().c_str(), txUtility::getFileName(cmd->getFile()).c_str(), cmd->getLine());
	}
	if (cmd->isDelayCommand())
	{
		DelayCommand delayCommand(delayExecute, cmd, cmdReceiver);

		// 等待解锁缓冲区,锁定缓冲区
		LOCK(mBufferLock, LT_WRITE);
		mCommandBufferInput.push_back(delayCommand);
		// 解锁缓冲区
		UNLOCK(mBufferLock, LT_WRITE);
	}
	else
	{
		GAME_ERROR("error : cmd is not a delay command, please use Command::createDelayCommand to create a delay command! \n\
command type : %s, file : %s, line : %d", typeid(*cmd).name(), cmd->getFile(), cmd->getLine());
	}
}

void txCommandSystem::destroy()
{
	int inputSize = mCommandBufferInput.size();
	for (int i = 0; i < inputSize; ++i)
	{
		TRACE_DELETE(mCommandBufferInput[i].mCommand);
	}
	mCommandBufferInput.clear();

	int processSize = mCommandBufferProcess.size();
	for (int i = 0; i < processSize; ++i)
	{
		TRACE_DELETE(mCommandBufferProcess[i].mCommand);
	}
	mCommandBufferProcess.clear();
}

void txCommandSystem::notifyReceiverDestroied(txCommandReceiver* receiver)
{
	// 等待解锁缓冲区,锁定缓冲区
	LOCK(mBufferLock, LT_WRITE);
	std::vector<DelayCommand>::iterator iterCommandInput = mCommandBufferInput.begin();
	for (; iterCommandInput != mCommandBufferInput.end();)
	{
		if (iterCommandInput->mReceiver == receiver)
		{
			TRACE_DELETE(iterCommandInput->mCommand);
			iterCommandInput = mCommandBufferInput.erase(iterCommandInput);
		}
		else
		{
			++iterCommandInput;
		}
	}
	// 解锁缓冲区
	UNLOCK(mBufferLock, LT_WRITE);

	std::vector<DelayCommand>::iterator iterCommand = mCommandBufferProcess.begin();
	for (; iterCommand != mCommandBufferProcess.end();)
	{
		if (iterCommand->mReceiver == receiver)
		{
			TRACE_DELETE(iterCommand->mCommand);
			iterCommand = mCommandBufferProcess.erase(iterCommand);
		}
		else
		{
			++iterCommand;
		}
	}
}