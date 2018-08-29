#ifndef _SERVER_BASE_H_
#define _SERVER_BASE_H_

#include "ServerDefine.h"

class ServerFramework;
class ServerConfig;
class txCommandSystem;
class NetServer;
class CharacterManager;
class RoomManager;
class MySQLDataBase;
class txComponentFactoryManager;
class DataBase;
class GameLog;
class MahjongRobotManager;
class MatchSystem;
class txCommand;
class txCommandReceiver;
class Packet;
class CharacterPlayer;
class NetClient;
class ServerBase
{
public:
	static void notifyConstructDone();
	static void notifyComponentDeconstruct();
	// 在任意线程中发送立即执行的命令,则该命令将在该线程中执行
	static void pushCommand(txCommand* cmd, txCommandReceiver* cmdReceiver);
	// 延迟执行的命令都会在主线程中执行
	static void pushDelayCommand(txCommand* cmd, txCommandReceiver* cmdReceiver, float delayExecute = 0.001f);
	static void sendMessage(Packet* packet, CharacterPlayer* player, bool destroyPacketEndSend = true);
	static void sendMessage(Packet* packet, CLIENT_GUID clientGUID, bool destroyPacketEndSend = true);
	static void sendMessage(Packet* packet, NetClient* client, bool destroyPacketEndSend = true);
public:
	static ServerFramework* mServerFramework;
	static ServerConfig* mServerConfig;
	static txCommandSystem* mCommandSystem;
	static NetServer* mNetServer;
	static CharacterManager* mCharacterManager;
	static RoomManager* mRoomManager;
	static MySQLDataBase* mMySQLDataBase;
	static txComponentFactoryManager* mComponentFactoryManager;
	static DataBase* mDataBase;
	static GameLog* mGameLog;
	static MahjongRobotManager* mMahjongRobotManager;
	static MatchSystem* mMatchSystem;
};

#endif
