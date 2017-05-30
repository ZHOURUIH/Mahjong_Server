#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetManagerServer.h"

void CSCheckAccount::execute()
{
	// 查询并立即返回结果
	bool ret = mMySQLDataBase->isAccountExist(mAccount);
	SCCheckAccountRet* checkRet = static_cast<SCCheckAccountRet*>(mNetManagerServer->createPacket(PT_SC_CHECK_ACCOUNT));
	checkRet->mResult = ret ? 1 : 0;
	mNetManagerServer->sendMessage(checkRet, mClient);
}