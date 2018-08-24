#include "PacketHeader.h"
#include "MySQLDataBase.h"
#include "NetServer.h"
#include "SQLAccount.h"
#include "SQLCharacterData.h"
#include "Utility.h"

void CSRegister::execute()
{
	// 玩家注册,向数据库查询
	AccountTable* dataAccount = TRACE_NEW(AccountTable, dataAccount);
	dataAccount->mAccount = mAccount;
	dataAccount->mPassword = mPassword;
	dataAccount->mIsRobot = false;
	CharacterDataTable* dataCharacterData = TRACE_NEW(CharacterDataTable, dataCharacterData);
	dataCharacterData->mName = mName;
	dataCharacterData->mHead = mHead;
	dataCharacterData->mMoney = 0;
	int ret = mMySQLDataBase->registerAccount(dataAccount, dataCharacterData);
	TRACE_DELETE(dataAccount);
	TRACE_DELETE(dataCharacterData);
	// 直接发回注册结果
	SCRegisterRet* registerRet = NetServer::createPacket(registerRet, PT_SC_REGISTER_RET);
	registerRet->mResult = ret;
	mNetServer->sendMessage(registerRet, mClient);
}
