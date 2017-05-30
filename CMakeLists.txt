#project name
PROJECT(NetBikeServer)

#minimum_version
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-g")

SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

INCLUDE(CheckCXXCompilerFlag)  
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)  
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)  
IF(COMPILER_SUPPORTS_CXX11)  
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	MESSAGE("c++11")
ELSEIF(COMPILER_SUPPORTS_CXX0X)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	MESSAGE("c++0x")
ELSE()  
     MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")  
ENDIF() 

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#静态库路径
LINK_DIRECTORIES(/usr/lib/mysql/plugin)
LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu)

#添加平台宏
IF(WIN32)
MESSAGE("this is windows!")
ADD_DEFINITIONS(-DWINDOWS)
ENDIF(WIN32)

IF(UNIX)
MESSAGE("this is linux!")
ADD_DEFINITIONS(-DLINUX)
ENDIF(UNIX)

ADD_DEFINITIONS(-D_SERVER)

#head file path
INCLUDE_DIRECTORIES(
Server/Character;
Server/CommandSystem;
Server/CommandSystem/CommandCharacter;
Server/CommandSystem/CommandCharacterManager;
Server/CommandSystem/CommandRoom;
Server/CommandSystem/CommandRoomManager;
Server/CommandSystem/CommandServer;
Server/Common;
Server/Component;
Server/Component/BaseComponent;
Server/DataBase;
Server/FileSystem;
Server/Net;
Server/Room;
Server/ServerFramework;
Server/Socket;
Server/Socket/Packet;
Server/Socket/Packet/ClientServer;
Server/Socket/Packet/ServerClient;
IF(WIN32)
$(MYSQL_HOME)/include;
ENDIF(WIN32)
IF(UNIX)
/usr/include/mysql;
ENDIF
)

SET(SRC_LIST 
Server/Character/Character.cpp;
Server/Character/CharacterData.cpp;
Server/Character/CharacterFactoryManager.cpp;
Server/Character/CharacterManager.cpp;
Server/Character/CharacterNPC.cpp;
Server/Character/CharacterPlayer.cpp;
Server/CommandSystem/txCommand.cpp;
Server/CommandSystem/txCommandReceiver.cpp;
Server/CommandSystem/txCommandSystem.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterContinueGame.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterJoinRoom.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterLeaveRoom.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyFinishGame.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyHardwareSpeedChanged.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyPlayerOffline.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifySyncPosition.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterStartGame.cpp;
Server/CommandSystem/CommandCharacterManager/CommandCharacterManagerCreateCharacter.cpp;
Server/CommandSystem/CommandCharacterManager/CommandCharacterManagerDestroyCharacter.cpp;
Server/CommandSystem/CommandCharacterManager/CommandCharacterManagerPlayerLogin.cpp;
Server/CommandSystem/CommandCharacterManager/CommandCharacterManagerNotifyPlayerOffline.cpp;
Server/CommandSystem/CommandRoom/CommandRoomEnterGame.cpp;
Server/CommandSystem/CommandRoom/CommandRoomNotifyPlayerHardwareSpeedChanged.cpp;
Server/CommandSystem/CommandRoom/CommandRoomNotifyPlayerOffline.cpp;
Server/CommandSystem/CommandRoom/CommandRoomNotifySyncPosition.cpp;
Server/CommandSystem/CommandRoom/CommandRoomPlayerContinueGame.cpp;
Server/CommandSystem/CommandRoom/CommandRoomPlayerFinishGame.cpp;
Server/CommandSystem/CommandRoom/CommandRoomPlayerLeave.cpp;
Server/CommandSystem/CommandRoom/CommandRoomPlayerReady.cpp;
Server/CommandSystem/CommandRoom/CommandRoomStartGame.cpp;
Server/CommandSystem/CommandRoomManager/CommandRoomManagerCancelMatch.cpp;
Server/CommandSystem/CommandRoomManager/CommandRoomManagerCreateRoom.cpp;
Server/CommandSystem/CommandRoomManager/CommandRoomManagerDestroyRoom.cpp;
Server/CommandSystem/CommandRoomManager/CommandRoomManagerRequestMatch.cpp;
Server/CommandSystem/CommandRoomManager/CommandRoomManagerNotifyPlayerOffline.cpp;
Server/CommandSystem/CommandServer/CommandServerNotifyAcceptedClient.cpp;
Server/Common/ServerBase.cpp;
Server/Common/ServerConfig.cpp;
Server/Common/txThreadLock.cpp;
Server/Common/txMemeryTrace.cpp;
Server/Common/txSerializedData.cpp;
Server/Common/txSerializer.cpp;
Server/Common/txShareMemeryServer.cpp;
Server/Common/txUtility.cpp;
Server/Component/txComponentFactoryManager.cpp;
Server/Component/txComponentOwner.cpp;
Server/Component/BaseComponent/txComponent.cpp;
Server/DataBase/Data.cpp;
Server/DataBase/DataBase.cpp;
Server/DataBase/MySQLDataBase.cpp;
Server/FileSystem/txDataElem.cpp;
Server/FileSystem/txDataManager.cpp;
Server/Net/NetManagerClient.cpp;
Server/Net/NetManagerServer.cpp;
Server/Room/Room.cpp;
Server/Room/RoomManager.cpp;
Server/ServerFramework/ServerFramework.cpp;
Server/Socket/PacketFactoryManager.cpp;
Server/Socket/Packet/ClientServer/CSCancelMatch.cpp;
Server/Socket/Packet/ClientServer/CSCharacterHardwareSpeedChanged.cpp;
Server/Socket/Packet/ClientServer/CSCheckAccount.cpp;
Server/Socket/Packet/ClientServer/CSCheckName.cpp;
Server/Socket/Packet/ClientServer/CSContinueGame.cpp;
Server/Socket/Packet/ClientServer/CSFinishGame.cpp;
Server/Socket/Packet/ClientServer/CSGameReady.cpp;
Server/Socket/Packet/ClientServer/CSHeartBeat.cpp;
Server/Socket/Packet/ClientServer/CSLogin.cpp;
Server/Socket/Packet/ClientServer/CSRegister.cpp;
Server/Socket/Packet/ClientServer/CSRequestMatch.cpp;
Server/Socket/Packet/ClientServer/CSSendPosition.cpp;
Server/Socket/Packet/ServerClient/SCCancelMatchRet.cpp;
Server/Socket/Packet/ServerClient/SCChangeCharacterHardwareSpeed.cpp;
Server/Socket/Packet/ServerClient/SCCheckAccountRet.cpp;
Server/Socket/Packet/ServerClient/SCCheckNameRet.cpp;
Server/Socket/Packet/ServerClient/SCContinueGameRet.cpp;
Server/Socket/Packet/ServerClient/SCFinishGameRet.cpp;
Server/Socket/Packet/ServerClient/SCHeartBeatRet.cpp;
Server/Socket/Packet/ServerClient/SCLoginRet.cpp;
Server/Socket/Packet/ServerClient/SCMatchSuccess.cpp;
Server/Socket/Packet/ServerClient/SCPlayerOffline.cpp;
Server/Socket/Packet/ServerClient/SCRegisterRet.cpp;
Server/Socket/Packet/ServerClient/SCStartGame.cpp;
Server/Socket/Packet/ServerClient/SCSyncPosition.cpp;
Server/main.cpp)

#source directory
AUX_SOURCE_DIRECTORY(Server SRC_LIST)

#add executable file
ADD_EXECUTABLE(NetBikeServer ${SRC_LIST})

#链接静态库
TARGET_LINK_LIBRARIES(NetBikeServer libmysqlclient.a -ldl -lpthread -lz)

