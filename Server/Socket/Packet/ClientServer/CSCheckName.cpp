#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"

void CSCheckName::execute()
{
	// 查询并立即返回结果
	bool ret = mMySQLDataBase->isNameExist(mName);
	SCCheckNameRet* checkRet = NetServer::createPacket(checkRet, PT_SC_CHECK_NAME_RET);
	checkRet->mResult = ret ? 1 : 0;
	mNetServer->sendMessage(checkRet, mClient);
}