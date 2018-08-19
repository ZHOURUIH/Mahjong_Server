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
class ServerBase
{
public:
	static void notifyConstructDone();
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
};

#endif
