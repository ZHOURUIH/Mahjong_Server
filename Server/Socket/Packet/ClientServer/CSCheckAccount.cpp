#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"

void CSCheckAccount::execute()
{
	// 查询并立即返回结果
	bool ret = mMySQLDataBase->isAccountExist(mAccount);
	SCCheckAccountRet* checkRet = static_cast<SCCheckAccountRet*>(mNetServer->createPacket(PT_SC_CHECK_ACCOUNT_RET));
	checkRet->mResult = ret ? 1 : 0;
	mNetServer->sendMessage(checkRet, mClient);
}