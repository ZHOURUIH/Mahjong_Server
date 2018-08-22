#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"

void CSRegister::execute()
{
	// 玩家注册,向数据库查询
	int ret = mMySQLDataBase->registerAccount(mAccount, mPassword, mName, 0, mHead, false);
	// 直接发回注册结果
	SCRegisterRet* registerRet = NetServer::createPacket(registerRet, PT_SC_REGISTER_RET);
	registerRet->mResult = ret;
	mNetServer->sendMessage(registerRet, mClient);
}
