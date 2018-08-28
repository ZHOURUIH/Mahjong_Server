#include "ServerBase.h"
#include "ServerFramework.h"

ServerFramework* ServerBase::mServerFramework = NULL;
ServerConfig* ServerBase::mServerConfig = NULL;
txCommandSystem* ServerBase::mCommandSystem = NULL;
NetServer* ServerBase::mNetServer = NULL;
CharacterManager* ServerBase::mCharacterManager = NULL;
RoomManager* ServerBase::mRoomManager = NULL;
MySQLDataBase* ServerBase::mMySQLDataBase = NULL;
txComponentFactoryManager* ServerBase::mComponentFactoryManager = NULL;
DataBase* ServerBase::mDataBase = NULL;
GameLog* ServerBase::mGameLog = NULL;
MahjongRobotManager* ServerBase::mMahjongRobotManager = NULL;
MatchSystem* ServerBase::mMatchSystem = NULL;

void ServerBase::notifyConstructDone()
{
	mServerFramework = ServerFramework::getSingletonPtr();
	mServerConfig = mServerFramework->GET_SYSTEM(ServerConfig);
	mCommandSystem = mServerFramework->GET_SYSTEM(txCommandSystem);
	mNetServer = mServerFramework->GET_SYSTEM(NetServer);
	mCharacterManager = mServerFramework->GET_SYSTEM(CharacterManager);
	mRoomManager = mServerFramework->GET_SYSTEM(RoomManager);
	mMySQLDataBase = mServerFramework->GET_SYSTEM(MySQLDataBase);
	mComponentFactoryManager = mServerFramework->GET_SYSTEM(txComponentFactoryManager);
	mDataBase = mServerFramework->GET_SYSTEM(DataBase);
	mGameLog = mServerFramework->GET_SYSTEM(GameLog);
	mMahjongRobotManager = mServerFramework->GET_SYSTEM(MahjongRobotManager);
	mMatchSystem = mServerFramework->GET_SYSTEM(MatchSystem);
}

void ServerBase::notifyComponentDeconstruct()
{
	// 重新再获取一下所有组件
	notifyConstructDone();
}