#include "PacketHeader.h"
#ifdef _SERVER
#include "NetManagerServer.h"
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
	std::string playerName;
	CHAR_GUID guid;
	bool qeuryRet = mMySQLDataBase->queryLogin(mAccount, mPassword, playerName, guid);
	int ret = 1;
	// 账号不存在或者密码错误
	if (!qeuryRet)
	{
		ret = 0;
	}
	// 已在其他地方登录
	else if (mCharacterManager->isCharacterLogin(guid))
	{
		ret = -1;
	}
	// 如果登陆失败,则立即发送消息
	if (ret != 1)
	{
		SCLoginRet* loginRet = static_cast<SCLoginRet*>(mNetManagerServer->createPacket(PT_SC_LOGIN_RET));
		loginRet->mLoginRet = ret;
		mNetManagerServer->sendMessage(loginRet, mClient);
	}
	// 登陆成功,则先创建角色,角色创建完成后再发送消息
	else
	{
		CommandCharacterManagerPlayerLogin cmdLogin(COMMAND_PARAM);
		cmdLogin.mClient = mClient;
		cmdLogin.mGUID = guid;
		cmdLogin.mName = playerName;
		mCommandSystem->pushCommand(&cmdLogin, mCharacterManager);
	}
#endif
}
