#ifndef _FRAME_BASE_H_
#define _FRAME_BASE_H_

#include "ServerDefine.h"
#include "SystemUtility.h"

class ServerFramework;
class FrameConfig;
class CommandSystem;
class NetServer;
class CharacterManager;
class ComponentFactoryManager;
class GameLog;
class txCommand;
class CommandReceiver;
class Packet;
class CharacterPlayer;
class NetClient;
class InputSystem;
class FrameBase : public SystemUtility
{
public:
	virtual void notifyConstructDone();
	virtual void notifyComponentDeconstruct();
	// 在任意线程中发送立即执行的命令,则该命令将在该线程中执行
	static void pushCommand(txCommand* cmd, CommandReceiver* cmdReceiver);
	// 延迟执行的命令都会在主线程中执行
	static void pushDelayCommand(txCommand* cmd, CommandReceiver* cmdReceiver, float delayExecute = 0.001f);
	static void sendMessage(Packet* packet, IClient* player, bool destroyPacketEndSend = true);
	static void sendMessage(Packet* packet, CLIENT_GUID clientGUID, bool destroyPacketEndSend = true);
	static void sendMessage(Packet* packet, NetClient* client, bool destroyPacketEndSend = true);
public:
	static ServerFramework* mServerFramework;
	static FrameConfig* mFrameConfig;
	static CommandSystem* mCommandSystem;
	static NetServer* mNetServer;
	static CharacterManager* mCharacterManager;
	static ComponentFactoryManager* mComponentFactoryManager;
	static GameLog* mGameLog;
	static InputSystem* mInputSystem;
};

#endif
