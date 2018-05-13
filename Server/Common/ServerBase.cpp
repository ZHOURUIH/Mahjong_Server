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

void ServerBase::notifyConstructDone()
{
	mServerFramework = ServerFramework::getSingletonPtr();
	mServerConfig = mServerFramework->getServerConfig();
	mCommandSystem = mServerFramework->getCommandSystem();
	mNetServer = mServerFramework->getNetManagerServer();
	mCharacterManager = mServerFramework->getCharacterManager();
	mRoomManager = mServerFramework->getRoomManager();
	mMySQLDataBase = mServerFramework->getMySQLDataBase();
	mComponentFactoryManager = mServerFramework->getComponentFactoryManager();
}
