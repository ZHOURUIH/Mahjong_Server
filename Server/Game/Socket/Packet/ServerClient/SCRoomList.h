#ifndef _SC_ROOM_LIST_H_
#define _SC_ROOM_LIST_H_

#include "Packet.h"
#include "GameDefine.h"
#include "Utility.h"
#include "Room.h"

class SCRoomList : public Packet
{
public:
	SCRoomList(PACKET_TYPE type)
		:Packet(type) {}
	virtual void fillParams()
	{
		pushParam(mAllRoomCount);
		pushParam(mRoomCount);
		pushArrayParam(mRoomID, MAX_REQUEST_ROOM_COUNT);
		pushArrayParam(mOwnerName, MAX_NAME_LENGTH * MAX_REQUEST_ROOM_COUNT);
		pushArrayParam(mCurCount, MAX_REQUEST_ROOM_COUNT);
		pushArrayParam(mMaxCount, MAX_REQUEST_ROOM_COUNT);
	}
	void setAllRoomCount(int allCount)
	{
		mAllRoomCount = allCount;
	}
	void setRoomList(txVector<Room*>& roomList)
	{
		mRoomCount = roomList.size();
		MathUtility::clamp(mRoomCount, (char)0, (char)MAX_REQUEST_ROOM_COUNT);
		FOR(roomList, int i = 0; i < mRoomCount; ++i)
		{
			Room* room = roomList[i];
			mRoomID[i] = room->getID();
			const std::string& name = room->getBanker()->getName();
			memcpy(mOwnerName + i * MAX_NAME_LENGTH, name.c_str(), name.length());
			mCurCount[i] = room->getPlayerList().size();
			mMaxCount[i] = room->getMaxPlayerCount();
		}
		END(roomList);
	}
public:
	short mAllRoomCount;
	char mRoomCount;
	int mRoomID[MAX_REQUEST_ROOM_COUNT];
	char mOwnerName[MAX_NAME_LENGTH * MAX_REQUEST_ROOM_COUNT];
	char mCurCount[MAX_REQUEST_ROOM_COUNT];
	char mMaxCount[MAX_REQUEST_ROOM_COUNT];
};

#endif