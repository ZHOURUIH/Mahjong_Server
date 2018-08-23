#include "MahjongRobotManager.h"
#include "CommandHeader.h"
#include "CharacterManager.h"
#include "CharacterMahjongRobot.h"
#include "CharacterData.h"
#include "MySQLDataBase.h"

CharacterMahjongRobot* MahjongRobotManager::createRobot()
{
	CharacterMahjongRobot* robot = NULL;
	// �����Ƿ����Ѿ�������û���ڷ����еĻ�����
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

	if (robot == NULL)
	{
		// �����ݿ��в�ѯ�Ƿ��п��õĻ������˺�,û�п��õ��˺ž�ע��������˺�
		std::string account, password;
		if (!mMySQLDataBase->getFirstNotLoginAccount(account, password))
		{
			bool registeRet = false;
			// ���ֻ����10��
			for(int i = 0; i < 10; ++i)
			{
				account = generateRobotAccount();
				password = generateRobotPassword();
				int registeRet = mMySQLDataBase->registerAccount(account, password, generateRobotName(), 100, 0, true);
				if (registeRet == 0)
				{
					registeRet = true;
					break;
				}
			}
			if (!registeRet)
			{
				LOG_ERROR("registe robot failed!");
			}
		}
		// ��¼�������˺�,���GUID
		CHAR_GUID guid;
		mMySQLDataBase->queryLogin(account, password, guid, true);
		// ��¼������
		std::string playerName;
		int money = 0;
		short head = 0;
		mMySQLDataBase->queryCharacterData(guid, playerName, money, head);
		CommandCharacterManagerRobotLogin* cmdLogin = NEW_CMD_INFO(cmdLogin);
		cmdLogin->mGUID = guid;
		cmdLogin->mName = playerName;
		cmdLogin->mMoney = money;
		cmdLogin->mHead = head;
		mCommandSystem->pushCommand(cmdLogin, mCharacterManager);
		robot = static_cast<CharacterMahjongRobot*>(mCharacterManager->getCharacter(guid));
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