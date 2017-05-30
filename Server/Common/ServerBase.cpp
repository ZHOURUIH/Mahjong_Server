#include "ServerBase.h"
#include "ServerFramework.h"

ServerFramework* ServerBase::mServerFramework = NULL;
ServerConfig* ServerBase::mServerConfig = NULL;
txDataManager* ServerBase::mDataManager = NULL;
txCommandSystem* ServerBase::mCommandSystem = NULL;
NetManagerServer* ServerBase::mNetManagerServer = NULL;
CharacterManager* ServerBase::mCharacterManager = NULL;
RoomManager* ServerBase::mRoomManager = NULL;
MySQLDataBase* ServerBase::mMySQLDataBase = NULL;
txComponentFactoryManager* ServerBase::mComponentFactoryManager = NULL;

void ServerBase::notifyConstructDone()
{
	mServerFramework = ServerFramework::getSingletonPtr();
	mServerConfig = mServerFramework->getServerConfig();
	mDataManager = mServerFramework->getDataManager();
	mCommandSystem = mServerFramework->getCommandSystem();
	mNetManagerServer = mServerFramework->getNetManagerServer();
	mCharacterManager = mServerFramework->getCharacterManager();
	mRoomManager = mServerFramework->getRoomManager();
	mMySQLDataBase = mServerFramework->getMySQLDataBase();
	mComponentFactoryManager = mServerFramework->getComponentFactoryManager();
}
