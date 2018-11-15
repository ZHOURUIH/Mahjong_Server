#include "CommandReceiver.h"
#include "txCommand.h"
#include "CommandSystem.h"
#include "ServerFramework.h"

CommandReceiver::CommandReceiver(const string& name)
:
mName(name)
{}

CommandReceiver::~CommandReceiver()
{
	// 通知命令系统有一个命令接受者已经被销毁了,需要取消命令缓冲区中的即将发给该接受者的命令
	// 此时需要确认引擎和命令系统没有被销毁
	ServerFramework* framework = ServerFramework::getSingletonPtr();
	if (framework != NULL)
	{
		CommandSystem* commandSystem = framework->getSystem<CommandSystem>(TOSTRING(CommandSystem));
		if (commandSystem != NULL && commandSystem != this)
		{
			commandSystem->notifyReceiverDestroied(this);
		}
	}
}

void CommandReceiver::receiveCommand(txCommand* cmd)
{
	cmd->runStartCallBack();
	cmd->setExecuteState(ES_EXECUTING);
	cmd->execute();
	cmd->setExecuteState(ES_EXECUTED);
	cmd->runEndCallBack();
}