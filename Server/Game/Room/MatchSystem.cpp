#include "MatchSystem.h"
#include "CharacterPlayer.h"
#include "CharacterData.h"
#include "RoomManager.h"
#include "txMemoryTrace.h"
#include "Utility.h"
#include "CommandHeader.h"
#include "Room.h"
#include "MahjongRobotManager.h"
#include "CharacterMahjongRobot.h"

void MatchSystem::init()
{
	;
}

void MatchSystem::update(float elapsedTime)
{
	// 更新自由匹配
	int count = mFreeMatchPool.size();
	if (count > 0)
	{
		// 获取当前所有未满的房间列表,然后将玩家随机匹配到房间中
		txVector<Room*> notFullRoomList;
		mRoomManager->getNotFullPublicRoomList(notFullRoomList);
		auto iterMatch = mFreeMatchPool.begin();
		FOR(mFreeMatchPool, ; iterMatch != mFreeMatchPool.end(); )
		{
			if (notFullRoomList.size() == 0)
			{
				break;
			}
			int index = MathUtility::randomInt(0, notFullRoomList.size() - 1);
			CommandCharacterJoinRoom* cmdJoin = NEW_CMD_INFO(cmdJoin);
			cmdJoin->mRoomID = notFullRoomList[index]->getID();
			pushCommand(cmdJoin, (*iterMatch)->mPlayer);
			mFreeMatchPoolMap.tryErase((*iterMatch)->mPlayer->getCharacterData()->mGUID);
			iterMatch = mFreeMatchPool.erase(iterMatch, false);
		}
		END(mFreeMatchPool);
		// 剩余未匹配的玩家需要累计等待时间
		count = mFreeMatchPool.size();
		iterMatch = mFreeMatchPool.begin();
		FOR(mFreeMatchPool, ; iterMatch != mFreeMatchPool.end(); )
		{
			bool removed = false;
			MatchInfo* info = *iterMatch;
			info->mWaitTime += elapsedTime;
			// 如果等待时间已经太长,则匹配机器人房间
			if (info->mWaitTime >= MAX_FREE_MATCH_WAIT_TIME)
			{
				Room* room = NULL;
				CommandRoomManagerCreateRoom* cmdCreatRoom = NEW_CMD_INFO(cmdCreatRoom);
				cmdCreatRoom->mRoomPtr = &room;
				pushCommand(cmdCreatRoom, mRoomManager);
				if (room != NULL)
				{
					// 将机器人加入到房间中,需要首先加入一个机器人,作为房主
					CharacterMahjongRobot* robot = mMahjongRobotManager->createRobot();
					CommandCharacterJoinRoom* cmdJoin0 = NEW_CMD_INFO(cmdJoin0);
					cmdJoin0->mRoomID = room->getID();
					pushCommand(cmdJoin0, robot);
					// 机器人默认准备状态
					CommandRoomPlayerReady* cmdReady = NEW_CMD_INFO(cmdReady);
					cmdReady->mReady = true;
					cmdReady->mPlayerGUID = robot->getGUID();
					pushCommand(cmdReady, room);
					// 再加入玩家
					CommandCharacterJoinRoom* cmdJoin1 = NEW_CMD_INFO(cmdJoin1);
					cmdJoin1->mRoomID = room->getID();
					pushCommand(cmdJoin1, info->mPlayer);
					// 再加入机器人将房间填满
					while (true)
					{
						if (room->isFull())
						{
							break;
						}
						CharacterMahjongRobot* robot0 = mMahjongRobotManager->createRobot();
						CommandCharacterJoinRoom* cmdJoin2 = NEW_CMD_INFO(cmdJoin2);
						cmdJoin2->mRoomID = room->getID();
						pushCommand(cmdJoin2, robot0);
						// 机器人默认准备状态
						CommandRoomPlayerReady* cmdReady0 = NEW_CMD_INFO(cmdReady0);
						cmdReady0->mReady = true;
						cmdReady0->mPlayerGUID = robot0->getGUID();
						pushCommand(cmdReady0, room);
					}
					mFreeMatchPoolMap.tryErase((*iterMatch)->mPlayer->getCharacterData()->mGUID);
					iterMatch = mFreeMatchPool.erase(iterMatch, false);
					removed = true;
				}
			}
			if (!removed)
			{
				++iterMatch;
			}
		}
		END(mFreeMatchPool);
	}
}

void MatchSystem::requestFreeMatch(CharacterPlayer* player)
{
	if (mFreeMatchPoolMap.contains(player->getCharacterData()->mGUID))
	{
		return;
	}
	MatchInfo* info = TRACE_NEW(MatchInfo, info);
	info->mPlayer = player;
	mFreeMatchPoolMap.insert(player->getCharacterData()->mGUID, info);
	mFreeMatchPool.push_back(info);
}

void MatchSystem::notifyPlayerOffline(CharacterPlayer* player)
{
	if (!mFreeMatchPoolMap.tryErase(player->getCharacterData()->mGUID))
	{
		return;
	}
	int count = mFreeMatchPool.size();
	FOR(mFreeMatchPool, int i = 0; i < count; ++i)
	{
		if (mFreeMatchPool[i]->mPlayer == player)
		{
			TRACE_DELETE(mFreeMatchPool[i]);
			mFreeMatchPool.erase(i);
			break;
		}
	}
	END(mFreeMatchPool);
}