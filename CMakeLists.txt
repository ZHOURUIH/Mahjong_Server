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
Server/Character/CharacterComponent;
Server/CommandSystem;
Server/CommandSystem/CommandCharacter;
Server/CommandSystem/CommandCharacterManager;
Server/CommandSystem/CommandGameScene;
Server/CommandSystem/CommandGameSceneManager;
Server/CommandSystem/CommandRoom;
Server/CommandSystem/CommandRoomManager;
Server/CommandSystem/CommandServer;
Server/Common;
Server/Component;
Server/Component/BaseComponent;
Server/DataBase;
Server/Memory;
Server/Net;
Server/Room;
Server/Serialize;
Server/ServerFramework;
Server/Socket;
Server/Socket/Packet;
Server/Socket/Packet/ClientServer;
Server/Socket/Packet/ServerClient;
Server/STL;
Server/Thread;
Server/Utility;
IF(WIN32)
$(MYSQL_HOME)/include;
ENDIF(WIN32)
IF(UNIX)
/usr/include/mysql;
ENDIF
)

SET(SRC_LIST 
Server/Character/Character.cpp;
Server/Character/CharacterFactoryManager.cpp;
Server/Character/CharacterManager.cpp;
Server/Character/CharacterNPC.cpp;
Server/Character/CharacterPlayer.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterAskAction.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterAskDrop.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterContinueGame.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterDrop.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterGang.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterGetMahjong.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterGetStartMahjong.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterJoinRoom.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterLeaveRoom.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterMahjongEnd.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyBanker.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyDiceDone.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyGetStartDone.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerAskAction.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerAskDrop.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerContinueGame.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerDrop.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerGang.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerGetMahjong.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerGetStartMahjong.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerJoinRoom.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerLeaveRoom.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerOffline.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerPass.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerPeng.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerReady.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerReorderMahjong.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterNotifyOtherPlayerShowHua.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterPass.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterPeng.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterPlayerHu.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterReady.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterReorderMahjong.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterShowHua.cpp;
Server/CommandSystem/CommandCharacter/CommandCharacterStartGame.cpp;
Server/CommandSystem/CommandCharacterManager/CommandCharacterManagerCreateCharacter.cpp;
Server/CommandSystem/CommandCharacterManager/CommandCharacterManagerDestroyCharacter.cpp;
Server/CommandSystem/CommandCharacterManager/CommandCharacterManagerNotifyPlayerOffline.cpp;
Server/CommandSystem/CommandCharacterManager/CommandCharacterManagerPlayerLogin.cpp;
Server/CommandSystem/CommandPool.cpp;
Server/CommandSystem/CommandRoom/CommandRoomConfirmAction.cpp;
Server/CommandSystem/CommandRoom/CommandRoomNotifyDiceDone.cpp;
Server/CommandSystem/CommandRoom/CommandRoomNotifyPlayerContinueGame.cpp;
Server/CommandSystem/CommandRoom/CommandRoomNotifyPlayerJoin.cpp;
Server/CommandSystem/CommandRoom/CommandRoomNotifyPlayerLeave.cpp;
Server/CommandSystem/CommandRoom/CommandRoomNotifyPlayerOffline.cpp;
Server/CommandSystem/CommandRoom/CommandRoomNotifyPlayerReady.cpp;
Server/CommandSystem/CommandRoom/CommandRoomRequestDrop.cpp;
Server/CommandSystem/CommandRoom/CommandRoomStartGame.cpp;
Server/CommandSystem/CommandRoomManager/CommandRoomManagerCreateRoom.cpp;
Server/CommandSystem/CommandRoomManager/CommandRoomManagerDestroyRoom.cpp;
Server/CommandSystem/CommandServer/CommandServerNotifyAcceptedClient.cpp;
Server/CommandSystem/txCommand.cpp;
Server/CommandSystem/txCommandReceiver.cpp;
Server/CommandSystem/txCommandSystem.cpp;
Server/Common/GameLog.cpp;
Server/Common/ServerBase.cpp;
Server/Common/ServerConfig.cpp;
Server/Component/BaseComponent/txComponent.cpp;
Server/Component/txComponentFactoryManager.cpp;
Server/Component/txComponentOwner.cpp;
Server/DataBase/Data.cpp;
Server/DataBase/DataBase.cpp;
Server/DataBase/MySQLDataBase.cpp;
Server/main.cpp;
Server/Memory/txMemoryCheck.cpp;
Server/Memory/txMemoryTrace.cpp;
Server/Memory/txShareMemoryServer.cpp;
Server/Net/NetClient.cpp;
Server/Net/NetServer.cpp;
Server/Room/Room.cpp;
Server/Room/RoomManager.cpp;
Server/Serialize/txSerializedData.cpp;
Server/Serialize/txSerializer.cpp;
Server/ServerFramework/ServerFramework.cpp;
Server/Socket/Packet/ClientServer/CSBackToMahjongHall.cpp;
Server/Socket/Packet/ClientServer/CSCheckAccount.cpp;
Server/Socket/Packet/ClientServer/CSCheckName.cpp;
Server/Socket/Packet/ClientServer/CSConfirmAction.cpp;
Server/Socket/Packet/ClientServer/CSContinueGame.cpp;
Server/Socket/Packet/ClientServer/CSCreateRoom.cpp;
Server/Socket/Packet/ClientServer/CSDiceDone.cpp;
Server/Socket/Packet/ClientServer/CSHeartBeat.cpp;
Server/Socket/Packet/ClientServer/CSJoinRoom.cpp;
Server/Socket/Packet/ClientServer/CSLeaveRoom.cpp;
Server/Socket/Packet/ClientServer/CSLogin.cpp;
Server/Socket/Packet/ClientServer/CSReady.cpp;
Server/Socket/Packet/ClientServer/CSRegister.cpp;
Server/Socket/Packet/ClientServer/CSRequestDrop.cpp;
Server/Socket/PacketFactoryManager.cpp;
Server/Thread/CustomThread.cpp;
Server/Thread/ThreadLock.cpp;
Server/Thread/TimeLock.cpp;
Server/Utility/ServerUtility.cpp;
Server/Utility/txFileUtility.cpp;
Server/Utility/txMathUtility.cpp;
Server/Utility/txStringUtility.cpp;
Server/Utility/txUtility.cpp)

#source directory
AUX_SOURCE_DIRECTORY(Server SRC_LIST)

#add executable file
ADD_EXECUTABLE(Mahjong_Server ${SRC_LIST})

#链接静态库
TARGET_LINK_LIBRARIES(Mahjong_Server libmysqlclient.a -ldl -lpthread -lz)

