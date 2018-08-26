﻿#ifndef _TX_COMMAND_SYSTEM_H_
#define _TX_COMMAND_SYSTEM_H_

#include "CommandDefine.h"
#include "ServerDefine.h"
#include "ThreadLock.h"
#include "CommandPool.h"
#include "FrameComponent.h"

class txCommand;
class txCommandSystem : public FrameComponent
{
public:
	txCommandSystem(const std::string& name);
	virtual ~txCommandSystem(){ destroy(); }
	virtual void init()
	{
		mCommandPool->init();
	}
	virtual void update(float elapsedTime);
	// 创建命令
	template<typename T>
	T* newCmd(T*& cmd, const std::string& file, int line, bool show = true, bool delay = false)
	{
		cmd = mCommandPool->newCmd<T>(show, delay);
		cmd->setFile(file);
		cmd->setLine(line);
		return cmd;
	}
	// 中断延时命令,在命令还未执行时,将命令销毁
	bool interruptCommand(int assignID);
	// 在任意线程中发送立即执行的命令,则该命令将在该线程中执行
	void pushCommand(txCommand* cmd, txCommandReceiver* cmdReceiver);
	// 延迟执行的命令都会在主线程中执行
	// delayExecute是命令延时执行的时间,默认为0.01,只有new出来的命令才能延时执行
	void pushDelayCommand(txCommand* cmd, txCommandReceiver* cmdReceiver, float delayExecute = 0.001f);
	virtual void notifyReceiverDestroied(txCommandReceiver* receiver);
protected:
	void destroy();
	void syncCommandBuffer();
protected:
	CommandPool* mCommandPool;
	txVector<DelayCommand*> mCommandBufferProcess;	// 用于处理的命令列表
	txVector<DelayCommand*> mCommandBufferInput;	// 用于放入命令的命令列表
	txVector<DelayCommand*> mExecuteList;			// 即将在这一帧执行的命令
	ThreadLock mBufferLock;
	bool mSystemDestroy;	// 命令系统是否已经销毁
};

#endif