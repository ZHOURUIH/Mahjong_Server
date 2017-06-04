#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetManagerServer.h"

void CSCheckName::execute()
{
	// 查询并立即返回结果
	bool ret = mMySQLDataBase->isNameExist(mName);
	SCCheckNameRet* checkRet = static_cast<SCCheckNameRet*>(mNetManagerServer->createPacket(PT_SC_CHECK_NAME_RET));
	checkRet->mResult = ret ? 1 : 0;
	mNetManagerServer->sendMessage(checkRet, mClient);
}