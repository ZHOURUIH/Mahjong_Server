﻿#ifndef _TX_COMMAND_SYSTEM_H_
#define _TX_COMMAND_SYSTEM_H_

#include "CommandDefine.h"
#include "ServerDefine.h"
#include "txThreadLock.h"

class txCommand;
class txCommandReceiver;
class txCommandSystem
{
public:
	txCommandSystem()
		:
		mShowDebugInfo(true)
	{}
	virtual ~txCommandSystem();
	virtual void init(const bool& showDebug = true);
	void setShowDebugInfo(const bool& show) { mShowDebugInfo = show; }
	const bool& getShowDebugInfo() { return mShowDebugInfo; }
	void update(const float& elapsedTime);
	void pushCommand(txCommand* cmd, txCommandReceiver* cmdReceiver);
	// delayExecute是命令延时执行的时间,默认为0,只有new出来的命令才能延时执行
	// 子线程中发出的命令必须是延时执行的命令!
	void pushDelayCommand(txCommand* cmd, txCommandReceiver* cmdReceiver, const float& delayExecute = 0.001f);
	void destroy();
	virtual void notifyReceiverDestroied(txCommandReceiver* receiver);
protected:
	txVector<DelayCommand> mCommandBufferProcess;	// 用于处理的命令列表
	txVector<DelayCommand> mCommandBufferInput;		// 用于放入命令的命令列表
	txThreadLock mBufferLock;
	bool mShowDebugInfo;
};

#endif