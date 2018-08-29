#include "PacketHeader.h"
#include "NetServer.h"
#include "DataPlayerInfo.h"
#include "CharacterManager.h"
#include "CommandHeader.h"
#include "MySQLDataBase.h"
#include "SQLAccount.h"
#include "SQLCharacterData.h"

void CSLogin::execute()
{
	// 玩家登陆成功后,通知网络管理器有玩家登陆
	// 查询数据库
	AccountTable* accountData = TRACE_NEW(AccountTable, accountData);
	accountData->mAccount = mAccount;
	accountData->mPassword = mPassword;
	accountData->mIsRobot = false;
	bool qeuryRet = mMySQLDataBase->queryLogin(accountData);
	int ret = 0;
	// 账号不存在或者密码错误
	if (!qeuryRet)
	{
		ret = 1;
	}
	// 已在其他地方登录
	else if (mCharacterManager->isCharacterLogin(accountData->mGUID))
	{
		ret = 2;
	}
	// 如果登陆失败,则立即发送消息
	if (ret != 0)
	{
		SCLoginRet* loginRet = NEW_PACKET(loginRet, PT_SC_LOGIN_RET);
		loginRet->mLoginRet = ret;
		sendMessage(loginRet, mClient);
	}
	// 登陆成功,则先创建角色,角色创建完成后再发送消息
	else
	{
		CharacterDataTable* characterData = TRACE_NEW(CharacterDataTable, characterData);
		mMySQLDataBase->queryCharacterData(accountData->mGUID, characterData);
		CommandCharacterManagerPlayerLogin* cmdLogin = NEW_CMD_INFO(cmdLogin);
		cmdLogin->mClientID = mClientID;
		cmdLogin->mGUID = accountData->mGUID;
		cmdLogin->mName = characterData->mName;
		cmdLogin->mMoney = characterData->mMoney;
		cmdLogin->mHead = characterData->mHead;
		pushCommand(cmdLogin, mCharacterManager);
		TRACE_DELETE(characterData);
	}
	TRACE_DELETE(accountData);
}
