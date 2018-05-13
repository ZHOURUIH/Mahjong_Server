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
	// 鐜╁鐧婚檰鎴愬姛鍚?閫氱煡缃戠粶绠＄悊鍣ㄦ湁鐜╁鐧婚檰
	// 鏌ヨ鏁版嵁搴?
	CHAR_GUID guid;
	bool qeuryRet = mMySQLDataBase->queryLogin(mAccount, mPassword, guid);
	int ret = 0;
	// 璐﹀彿涓嶅瓨鍦ㄦ垨鑰呭瘑鐮侀敊璇?
	if (!qeuryRet)
	{
		ret = 1;
	}
	// 宸插湪鍏朵粬鍦版柟鐧诲綍
	else if (mCharacterManager->isCharacterLogin(guid))
	{
		ret = 2;
	}
	// 濡傛灉鐧婚檰澶辫触,鍒欑珛鍗冲彂閫佹秷鎭?
	if (ret != 0)
	{
		SCLoginRet* loginRet = static_cast<SCLoginRet*>(mNetServer->createPacket(PT_SC_LOGIN_RET));
		loginRet->mLoginRet = ret;
		mNetServer->sendMessage(loginRet, mClient);
	}
	// 鐧婚檰鎴愬姛,鍒欏厛鍒涘缓瑙掕壊,瑙掕壊鍒涘缓瀹屾垚鍚庡啀鍙戦€佹秷鎭?
	else
	{
		std::string playerName;
		int money = 0;
		short head = 0;
		mMySQLDataBase->queryCharacterData(guid, playerName, money, head);
		CommandCharacterManagerPlayerLogin* cmdLogin = NEW_CMD(cmdLogin);
		cmdLogin->mClient = mClient;
		cmdLogin->mGUID = guid;
		cmdLogin->mName = playerName;
		cmdLogin->mMoney = money;
		cmdLogin->mHead = head;
		mCommandSystem->pushCommand(cmdLogin, mCharacterManager);
	}
#endif
}
