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

DebugSystem::DebugSystem(const std::string& name)
	:FrameComponent(name)
{
	mMemoryServer = TRACE_NEW(txShareMemoryServer, mMemoryServer);
	// 2MB�Ĺ����ڴ��С
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
	LOG_INFO("�ӿ���̨�յ����� : %s", param.c_str());
	txVector<std::string> paramList;
	StringUtility::split(param, "-", paramList);
	if (paramList[0] == "RoomList")
	{
		std::string allRoomInfo;
		auto& roomList = mRoomManager->getRoomList();
		allRoomInfo += "room count : " + StringUtility::intToString(roomList.size()) + "\r\n";
		auto iterRoom = roomList.begin();
		auto iterRoomEnd = roomList.end();
		FOR_STL(roomList, ; iterRoom != iterRoomEnd; ++iterRoom)
		{
			allRoomInfo += "room id : " + StringUtility::intToString(iterRoom->second->getID()) + " -> ";
			auto& playerList = iterRoom->second->getPlayerList();
			auto iterPlayer = playerList.begin();
			auto iterPlayerEnd = playerList.end();
			FOR_STL(playerList, ;  iterPlayer != iterPlayerEnd; ++iterPlayer)
			{
				allRoomInfo += "player id : " + StringUtility::intToString(iterPlayer->second->getCharacterData()->mGUID) + ", ";
				allRoomInfo += "player name : " + iterPlayer->second->getName() + ", ";
				NetClient* client = mNetServer->getClient(iterPlayer->second->getClientGUID());
				std::string ip = client != NULL ? client->getIP() : "null";
				allRoomInfo += "player ip : " + ip;
				allRoomInfo += ";  ";
			}
			END_FOR_STL(playerList);
			allRoomInfo += "\r\n";
		}
		END_FOR_STL(roomList);
		DATA_HEADER writeHeader;
		writeHeader.mCmd = DEBUG_SYSTEM_CMD;
		writeHeader.mDataSize = allRoomInfo.length() + 1;
		memcpy(writeHeader.mUserData, param.c_str(), mUserDataLength);
		mMemoryServer->WriteCmdData(writeHeader, allRoomInfo.c_str());
	}
	LOG_INFO("�ѷ�������");
}