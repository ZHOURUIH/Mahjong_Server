#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"

void CSRegister::execute()
{
	// 玩家注册,向数据库查询
	int ret = mMySQLDataBase->registerAccount(mAccount, mPassword, mName, 0, mHead);
	// 直接发回注册结果
	SCRegisterRet* registerRet = static_cast<SCRegisterRet*>(mNetServer->createPacket(PT_SC_REGISTER_RET));
	registerRet->mResult = ret;
	mNetServer->sendMessage(registerRet, mClient);
}
