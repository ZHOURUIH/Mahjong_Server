#include "DebugSystem.h"
#include "txShareMemoryServer.h"
#include "txMemoryTrace.h"
#include "Utility.h"
#include "RoomManager.h"
#include "Room.h"
#include "txSerializer.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "NetServer.h"
#include "NetClient.h"
#include "CharacterManager.h"

DebugSystem::DebugSystem(const std::string& name)
	:FrameComponent(name)
{
	mMemoryServer = TRACE_NEW(txShareMemoryServer, mMemoryServer);
	// 2MB的共享内存大小
	mBufferSize = 1024 * 1024 * 2;
	mBuffer = TRACE_NEW_ARRAY(char, mBufferSize, mBuffer);
}
void DebugSystem::destroy()
{
	;
}
void DebugSystem::init()
{
	mMemoryServer->Create("DebugSystem", mBufferSize + sizeof(DATA_HEADER));
}
void DebugSystem::update(float elapsedTime)
{
	DATA_HEADER header;
	header.mCmd = DEBUG_SYSTEM_CMD;
	header.mDataSize = mBufferSize;
	int ret = mMemoryServer->ReadCmdData(header, mBuffer);
	if (ret < 0)
	{
		LOG_ERROR("read share memory error : %d", GetLastError());
	}
	else if(ret > 0)
	{
		parseCmd(mBuffer);
	}
}

void DebugSystem::parseCmd(const std::string& param)
{
	LOG_INFO("从控制台收到数据 : %s", param.c_str());
	txVector<std::string> paramList;
	StringUtility::split(param, "-", paramList);
	std::string allInfo;
	if (paramList[0] == ROOM_LIST)
	{
		auto& roomList = mRoomManager->getRoomList();
		allInfo += "room count : " + StringUtility::intToString(roomList.size()) + "\r\n";
		auto iterRoom = roomList.begin();
		auto iterRoomEnd = roomList.end();
		FOR(roomList, ; iterRoom != iterRoomEnd; ++iterRoom)
		{
			allInfo += "room id : " + StringUtility::intToString(iterRoom->second->getID()) + " -> ";
			auto& playerList = iterRoom->second->getPlayerList();
			auto iterPlayer = playerList.begin();
			auto iterPlayerEnd = playerList.end();
			FOR(playerList, ;  iterPlayer != iterPlayerEnd; ++iterPlayer)
			{
				allInfo += "player id : " + StringUtility::intToString(iterPlayer->second->getCharacterData()->mGUID) + ", ";
				allInfo += "player name : " + iterPlayer->second->getName() + ", ";
				NetClient* client = mNetServer->getClient(iterPlayer->second->getClientGUID());
				std::string ip = client != NULL ? client->getIP() : "null";
				allInfo += "player ip : " + ip;
				allInfo += ";  ";
			}
			END(playerList);
			allInfo += "\r\n";
		}
		END(roomList);
	}
	else if (paramList[0] == PLAYER_LIST)
	{
		txMap<std::string, Character*> playerList;
		if (paramList.contains("p"))
		{
			mCharacterManager->getCharacterListByType(CT_PLAYER, playerList);
		}
		txMap<std::string, Character*> robotList;
		if (paramList.contains("r"))
		{
			mCharacterManager->getCharacterListByType(CT_MAHJONG_ROBOT, robotList);
		}
		auto iterRobot = robotList.begin();
		auto iterRobotEnd = robotList.end();
		FOR(robotList, ; iterRobot != iterRobotEnd; ++iterRobot)
		{
			playerList.insert(iterRobot->first, iterRobot->second);
		}
		END(robotList);

		allInfo = "player count : " + StringUtility::intToString(playerList.size()) + "\r\n";
		auto iterPlayer = playerList.begin();
		auto iterPlayerEnd = playerList.end();
		FOR(playerList, ; iterPlayer != iterPlayerEnd; ++iterPlayer)
		{
			CharacterPlayer* player = static_cast<CharacterPlayer*>(iterPlayer->second);
			allInfo += "player id : " + StringUtility::intToString(player->getCharacterData()->mGUID) + ", ";
			allInfo += "player name : " + player->getName() + ", ";
			NetClient* client = mNetServer->getClient(player->getClientGUID());
			std::string ip = client != NULL ? client->getIP() : "null";
			allInfo += "player ip : " + ip;
			allInfo += "\r\n";
		}
		END(playerList);
	}
	DATA_HEADER writeHeader;
	writeHeader.mCmd = DEBUG_SYSTEM_CMD;
	writeHeader.mDataSize = allInfo.length() + 1;
	memcpy(writeHeader.mUserData, param.c_str(), mUserDataLength);
	mMemoryServer->WriteCmdData(writeHeader, allInfo.c_str());
	LOG_INFO("已发送数据");
}