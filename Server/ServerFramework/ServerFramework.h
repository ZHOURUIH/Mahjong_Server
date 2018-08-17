#ifndef _SERVER_FRAMEWORK_H_
#define _SERVER_FRAMEWORK_H_

#include "txSingleton.h"

class DataBaseServer;
class ComponentFactoryBase;
class NetServer;
class txCommandSystem;
class CharacterManager;
class ServerConfig;
class RoomManager;
class MySQLDataBase;
class txComponentFactoryManager;
class ServerFramework : public txSingleton<ServerFramework>
{
public:
	ServerFramework();
	virtual ~ServerFramework();
	bool init();
	void update(float elapsedTime);
	void destroy();
	void launch();
	bool isStop() { return mStop; }
	void stop() { mStop = true; }
	// 获得成员变量
#if RUN_PLATFORM == PLATFORM_LINUX
	const unsigned long& getStartMiliTime() {return mStartMiliTime;}
#endif
	ServerConfig* getServerConfig() { return mServerConfig; }
	txCommandSystem* getCommandSystem() { return mCommandSystem; }
	NetServer* getNetManagerServer(){ return mNetServer; }
	CharacterManager* getCharacterManager() { return mCharacterManager; }
	RoomManager* getRoomManager() { return mRoomManager; }
	MySQLDataBase* getMySQLDataBase() { return mMySQLDataBase; }
	txComponentFactoryManager* getComponentFactoryManager() { return mComponentFactoryManager; }
protected:
	void initComponentFactory();
	void destroyComponentFactory();
protected:
#if RUN_PLATFORM == PLATFORM_LINUX
	unsigned long mStartMiliTime;
#endif
	bool mStop;
	MySQLDataBase* mMySQLDataBase;
	ServerConfig* mServerConfig;
	txCommandSystem* mCommandSystem;
	NetServer* mNetServer;
	CharacterManager* mCharacterManager;
	RoomManager* mRoomManager;
	txComponentFactoryManager* mComponentFactoryManager;
};

#endif
