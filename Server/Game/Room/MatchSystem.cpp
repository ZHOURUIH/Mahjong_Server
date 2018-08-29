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
	// ��������ƥ��
	int count = mFreeMatchPool.size();
	if (count > 0)
	{
		// ��ȡ��ǰ����δ���ķ����б�,Ȼ��������ƥ�䵽������
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
			mCommandSystem->pushCommand(cmdJoin, (*iterMatch)->mPlayer);
			mFreeMatchPoolMap.tryErase((*iterMatch)->mPlayer->getCharacterData()->mGUID);
			iterMatch = mFreeMatchPool.erase(iterMatch, false);
		}
		END(mFreeMatchPool);
		// ʣ��δƥ��������Ҫ�ۼƵȴ�ʱ��
		count = mFreeMatchPool.size();
		iterMatch = mFreeMatchPool.begin();
		FOR(mFreeMatchPool, ; iterMatch != mFreeMatchPool.end(); )
		{
			bool removed = false;
			MatchInfo* info = *iterMatch;
			info->mWaitTime += elapsedTime;
			// ����ȴ�ʱ���Ѿ�̫��,��ƥ������˷���
			if (info->mWaitTime >= MAX_FREE_MATCH_WAIT_TIME)
			{
				Room* room = NULL;
				CommandRoomManagerCreateRoom* cmdCreatRoom = NEW_CMD_INFO(cmdCreatRoom);
				cmdCreatRoom->mRoomPtr = &room;
				mCommandSystem->pushCommand(cmdCreatRoom, mRoomManager);
				if (room != NULL)
				{
					// �������˼��뵽������,��Ҫ���ȼ���һ��������,��Ϊ����
					CharacterMahjongRobot* robot = mMahjongRobotManager->createRobot();
					CommandCharacterJoinRoom* cmdJoin0 = NEW_CMD_INFO(cmdJoin0);
					cmdJoin0->mRoomID = room->getID();
					mCommandSystem->pushCommand(cmdJoin0, robot);
					// ������Ĭ��׼��״̬
					CommandCharacterReady* cmdReady = NEW_CMD_INFO(cmdReady);
					cmdReady->mReady = true;
					mCommandSystem->pushCommand(cmdReady, robot);
					// �ټ������
					CommandCharacterJoinRoom* cmdJoin1 = NEW_CMD_INFO(cmdJoin1);
					cmdJoin1->mRoomID = room->getID();
					mCommandSystem->pushCommand(cmdJoin1, info->mPlayer);
					// �ټ�������˽���������
					while (true)
					{
						if (room->isFull())
						{
							break;
						}
						CharacterMahjongRobot* robot0 = mMahjongRobotManager->createRobot();
						CommandCharacterJoinRoom* cmdJoin2 = NEW_CMD_INFO(cmdJoin2);
						cmdJoin2->mRoomID = room->getID();
						mCommandSystem->pushCommand(cmdJoin2, robot0);
						// ������Ĭ��׼��״̬
						CommandCharacterReady* cmdReady0 = NEW_CMD_INFO(cmdReady0);
						cmdReady0->mReady = true;
						mCommandSystem->pushCommand(cmdReady0, robot0);
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