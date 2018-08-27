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
	// ����Ҫȷ������������������128��
	int addRobotCount = 128 - mRobotAccountList.size();
	if (addRobotCount > 0)
	{
		for (int i = 0; i < addRobotCount; ++i)
		{
			registeRobot();
		}
	}
	// ��¼ȫ��������
	auto iter = mRobotAccountList.begin();
	auto iterEnd = mRobotAccountList.end();
	FOR(mRobotAccountList, ; iter != iterEnd; ++iter)
	{
		// ��¼������
		loginRobot(iter->first, false);
	}
	END(mRobotAccountList);
}

CharacterMahjongRobot* MahjongRobotManager::createRobot()
{
	CharacterMahjongRobot* robot = NULL;
	// �����Ƿ����Ѿ�������û���ڷ����еĻ�����
	auto iter = mRobotList.begin();
	auto iterEnd = mRobotList.end();
	FOR(mRobotList, ; iter != iterEnd; ++iter)
	{
		if (iter->second->getCharacterData()->mRoomID == INVALID_INT_ID)
		{
			robot = iter->second;
			break;
		}
	}
	END(mRobotList);

	// ����Ҳ������õĻ�����,��ע���µĻ�����,��Ϊ����ʱ�͵�¼��������ע��Ļ�����,���Դ˴�ֻ����ע���µĻ�����
	if (robot == NULL)
	{
		// һ��ע��32��������
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
		FOR(newRobotGUIDList, int i = 0; i < newRobotCount; ++i)
		{
			CharacterMahjongRobot* robot = loginRobot(newRobotGUIDList[i]);
			if (robot != NULL)
			{
				robotList.push_back(robot);
			}
		}
		END(newRobotGUIDList);
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
	FOR(mRobotList, ; iter != iterEnd; ++iter)
	{
		CommandCharacterManagerDestroyCharacter* cmd = NEW_CMD(cmd);
		cmd->mGUID = iter->second->getGUID();
		mCommandSystem->pushCommand(cmd, mCharacterManager);
	}
	END(mRobotList);

	auto iterAccount = mRobotAccountList.begin();
	auto iterAccountEnd = mRobotAccountList.end();
	FOR(mRobotAccountList, ; iterAccount != iterAccountEnd; ++iterAccount)
	{
		TRACE_DELETE(iterAccount->second);
	}
	END(mRobotAccountList);
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

CharacterMahjongRobot* MahjongRobotManager::loginRobot(CHAR_GUID guid, bool showInfo)
{
	if (mRobotList.contains(guid))
	{
		return mRobotList[guid];
	}
	// ��¼������
	CharacterDataTable* data = TRACE_NEW(CharacterDataTable, data);
	mMySQLDataBase->queryCharacterData(guid, data);
	CommandCharacterManagerRobotLogin* cmdLogin = NULL;
	if (showInfo)
	{
		cmdLogin = NEW_CMD_INFO(cmdLogin);
	}
	else
	{
		cmdLogin = NEW_CMD(cmdLogin);
	}
	cmdLogin->mGUID = guid;
	cmdLogin->mName = data->mName;
	cmdLogin->mMoney = data->mMoney;
	cmdLogin->mHead = data->mHead;
	mCommandSystem->pushCommand(cmdLogin, mCharacterManager);
	CharacterMahjongRobot* robot = static_cast<CharacterMahjongRobot*>(mCharacterManager->getCharacter(guid));
	mRobotList.insert(guid, robot);
	TRACE_DELETE(data);
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
		// ע��ɹ�������ѭ��,���ɹ��ͼ�������ע��
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