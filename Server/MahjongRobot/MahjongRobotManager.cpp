#include "MahjongRobotManager.h"
#include "CommandHeader.h"
#include "CharacterManager.h"
#include "CharacterMahjongRobot.h"
#include "CharacterData.h"
#include "MySQLDataBase.h"
#include "SQLAccount.h"
#include "SQLCharacterData.h"

void MahjongRobotManager::init()
{
	mMySQLDataBase->getAllRobotAccount(mRobotAccountList);
	// 至少要确保机器人数量不少于128个
	int addRobotCount = 128 - mRobotAccountList.size();
	if (addRobotCount > 0)
	{
		for (int i = 0; i < addRobotCount; ++i)
		{
			registeRobot();
		}
	}
	// 登录全部机器人
	auto iter = mRobotAccountList.begin();
	auto iterEnd = mRobotAccountList.end();
	FOR_STL(mRobotAccountList, ; iter != iterEnd; ++iter)
	{
		// 登录机器人
		loginRobot(iter->first);
	}
	END_FOR_STL(mRobotAccountList);
}

CharacterMahjongRobot* MahjongRobotManager::createRobot()
{
	CharacterMahjongRobot* robot = NULL;
	// 查找是否有已经创建的没有在房间中的机器人
	auto iter = mRobotList.begin();
	auto iterEnd = mRobotList.end();
	FOR_STL(mRobotList, ; iter != iterEnd; ++iter)
	{
		if (iter->second->getCharacterData()->mRoomID == INVALID_INT_ID)
		{
			robot = iter->second;
			break;
		}
	}
	END_FOR_STL(mRobotList);

	// 如果找不到可用的机器人,则注册新的机器人,因为启动时就登录了所有已注册的机器人,所以此处只能再注册新的机器人
	if (robot == NULL)
	{
		// 一次注册32个机器人
		txVector<CHAR_GUID> newRobotGUIDList;
		for (int i = 0; i < 32; ++i)
		{
			CHAR_GUID guid = registeRobot();
			if (guid != INVALID_ID)
			{
				newRobotGUIDList.push_back(guid);
			}
		}
		txVector<CharacterMahjongRobot*> robotList;
		int newRobotCount = newRobotGUIDList.size();
		FOR_STL(newRobotGUIDList, int i = 0; i < newRobotCount; ++i)
		{
			CharacterMahjongRobot* robot = loginRobot(newRobotGUIDList[i]);
			if (robot != NULL)
			{
				robotList.push_back(robot);
			}
		}
		END_FOR_STL(newRobotGUIDList);
		if (robotList.size() > 0)
		{
			robot = robotList[0];
		}
	}
	return robot;
}

void MahjongRobotManager::destroy()
{
	auto iter = mRobotList.begin();
	auto iterEnd = mRobotList.end();
	FOR_STL(mRobotList, ; iter != iterEnd; ++iter)
	{
		CommandCharacterManagerDestroyCharacter* cmd = NEW_CMD_INFO(cmd);
		cmd->mGUID = iter->second->getGUID();
		mCommandSystem->pushCommand(cmd, mCharacterManager);
	}
	END_FOR_STL(mRobotList);

	auto iterAccount = mRobotAccountList.begin();
	auto iterAccountEnd = mRobotAccountList.end();
	FOR_STL(mRobotAccountList, ; iterAccount != iterAccountEnd; ++iterAccount)
	{
		TRACE_DELETE(iterAccount->second);
	}
	END_FOR_STL(mRobotAccountList);
	mRobotAccountList.clear();
}

std::string MahjongRobotManager::generateRobotName()
{
	return "robot" + StringUtility::intToString(MathUtility::randomInt(0, 999999), 6);
}

std::string MahjongRobotManager::generateRobotAccount()
{
	return "robot" + StringUtility::intToString(MathUtility::randomInt(0, 999999), 6);
}

std::string MahjongRobotManager::generateRobotPassword()
{
	return "robot";
}

CharacterMahjongRobot* MahjongRobotManager::loginRobot(CHAR_GUID guid)
{
	if (mRobotList.contains(guid))
	{
		return mRobotList[guid];
	}
	// 登录机器人
	CharacterDataTable* data = TRACE_NEW(CharacterDataTable, data);
	mMySQLDataBase->queryCharacterData(guid, data);
	CommandCharacterManagerRobotLogin* cmdLogin = NEW_CMD_INFO(cmdLogin);
	cmdLogin->mGUID = guid;
	cmdLogin->mName = data->mName;
	cmdLogin->mMoney = data->mMoney;
	cmdLogin->mHead = data->mHead;
	mCommandSystem->pushCommand(cmdLogin, mCharacterManager);
	CharacterMahjongRobot* robot = static_cast<CharacterMahjongRobot*>(mCharacterManager->getCharacter(guid));
	mRobotList.insert(guid, robot);
	return robot;
}

CHAR_GUID MahjongRobotManager::registeRobot()
{
	bool ret = false;
	CHAR_GUID guid = INVALID_ID;
	AccountTable* dataAccount = TRACE_NEW(AccountTable, dataAccount);
	CharacterDataTable* dataCharacterData = TRACE_NEW(CharacterDataTable, dataCharacterData);
	for (int i = 0; i < 16; ++i)
	{
		dataAccount->mAccount = generateRobotAccount();
		dataAccount->mPassword = generateRobotPassword();
		dataAccount->mIsRobot = true;
		dataCharacterData->mName = generateRobotName();
		dataCharacterData->mMoney = 1000000;
		dataCharacterData->mHead = 0;
		// 注册成功就跳出循环,不成功就继续尝试注册
		int registeRet = mMySQLDataBase->registerAccount(dataAccount, dataCharacterData);
		if (registeRet == 0)
		{
			guid = dataAccount->mGUID;
			mRobotAccountList.insert(dataAccount->mGUID, dataAccount);
			ret = true;
			break;
		}
	}
	if (!ret)
	{
		LOG_ERROR("registe robot error");
		TRACE_DELETE(dataAccount);
	}
	TRACE_DELETE(dataCharacterData);
	return guid;
}