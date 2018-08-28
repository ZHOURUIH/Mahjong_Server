#ifndef _SERVER_BASE_H_
#define _SERVER_BASE_H_

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
class ServerBase
{
public:
	static void notifyConstructDone();
	static void notifyComponentDeconstruct();
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
