#include "PacketHeader.h"
#ifdef _SERVER
#include "NetServer.h"
#include "DataPlayerInfo.h"
#include "CharacterManager.h"
#include "CommandHeader.h"
#include "MySQLDataBase.h"
#endif

void CSLogin::execute()
{
#ifdef _SERVER
	// 玩家登陆成功后,通知网络管理器有玩家登陆
	// 查询数据库
	CHAR_GUID guid;
	bool qeuryRet = mMySQLDataBase->queryLogin(mAccount, mPassword, guid);
	int ret = 0;
	// 账号不存在或者密码错误
	if (!qeuryRet)
	{
		ret = 1;
	}
	// 已在其他地方登录
	else if (mCharacterManager->isCharacterLogin(guid))
	{
		ret = 2;
	}
	// 如果登陆失败,则立即发送消息
	if (ret != 0)
	{
		SCLoginRet* loginRet = NetServer::createPacket(loginRet, PT_SC_LOGIN_RET);
		loginRet->mLoginRet = ret;
		mNetServer->sendMessage(loginRet, mClient);
	}
	// 登陆成功,则先创建角色,角色创建完成后再发送消息
	else
	{
		std::string playerName;
		int money = 0;
		short head = 0;
		mMySQLDataBase->queryCharacterData(guid, playerName, money, head);
		CommandCharacterManagerPlayerLogin* cmdLogin = NEW_CMD_INFO(cmdLogin);
		cmdLogin->mClient = mClient;
		cmdLogin->mGUID = guid;
		cmdLogin->mName = StringUtility::UTF8ToANSI(playerName);
		cmdLogin->mMoney = money;
		cmdLogin->mHead = head;
		mCommandSystem->pushCommand(cmdLogin, mCharacterManager);
	}
#endif
}
