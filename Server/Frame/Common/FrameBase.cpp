#include "FrameBase.h"
#include "ServerFramework.h"
#include "CommandSystem.h"
#include "NetServer.h"
#include "FrameConfig.h"
#include "CharacterManager.h"
#include "MySQLDataBase.h"
#include "ComponentFactoryManager.h"
#include "GameLog.h"
#include "InputSystem.h"

ServerFramework* FrameBase::mServerFramework = NULL;
FrameConfig* FrameBase::mFrameConfig = NULL;
CommandSystem* FrameBase::mCommandSystem = NULL;
NetServer* FrameBase::mNetServer = NULL;
CharacterManager* FrameBase::mCharacterManager = NULL;
ComponentFactoryManager* FrameBase::mComponentFactoryManager = NULL;
GameLog* FrameBase::mGameLog = NULL;
InputSystem* FrameBase::mInputSystem = NULL;

void FrameBase::notifyConstructDone()
{
	mServerFramework = ServerFramework::getSingletonPtr();
	mServerFramework->GET_SYSTEM(FrameConfig);
	mServerFramework->GET_SYSTEM(CommandSystem);
	mServerFramework->GET_SYSTEM(NetServer);
	mServerFramework->GET_SYSTEM(CharacterManager);
	mServerFramework->GET_SYSTEM(ComponentFactoryManager);
	mServerFramework->GET_SYSTEM(GameLog);
	mServerFramework->GET_SYSTEM(InputSystem);
}

void FrameBase::notifyComponentDeconstruct()
{
	// 重新再获取一下所有组件
	notifyConstructDone();
}

// 在任意线程中发送立即执行的命令,则该命令将在该线程中执行
void FrameBase::pushCommand(txCommand* cmd, CommandReceiver* cmdReceiver)
{
	mCommandSystem->pushCommand(cmd, cmdReceiver);
}
// 延迟执行的命令都会在主线程中执行
void FrameBase::pushDelayCommand(txCommand* cmd, CommandReceiver* cmdReceiver, float delayExecute)
{
	mCommandSystem->pushDelayCommand(cmd, cmdReceiver, delayExecute);
}

void FrameBase::sendMessage(Packet* packet, IClient* player, bool destroyPacketEndSend)
{
	mNetServer->sendMessage(packet, player, destroyPacketEndSend);
}

void FrameBase::sendMessage(Packet* packet, CLIENT_GUID clientGUID, bool destroyPacketEndSend)
{
	mNetServer->sendMessage(packet, clientGUID, destroyPacketEndSend);
}

void FrameBase::sendMessage(Packet* packet, NetClient* client, bool destroyPacketEndSend)
{
	mNetServer->sendMessage(packet, client, destroyPacketEndSend);
}