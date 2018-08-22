#include "MySQLDataBase.h"
#include "Utility.h"
#include "ServerConfig.h"
#include "SQLAccount.h"
#include "SQLCharacterData.h"

const char* MySQLDataBase::DATABASE_MAHJONG_USERDATA = "mahjong_userdata";
const char* MySQLDataBase::TABLE_ACCOUNT = "account";
const char* MySQLDataBase::TABLE_CHARACTER_DATA = "character_data";

MySQLDataBase::MySQLDataBase(const std::string& name)
:
FrameComponent(name)
{
	mMySQL = TRACE_NEW(MYSQL, mMySQL);
	mSQLAccount = TRACE_NEW(SQLAccount, mSQLAccount, TABLE_ACCOUNT);
	mSQLCharacterData = TRACE_NEW(SQLCharacterData, mSQLCharacterData, TABLE_CHARACTER_DATA);
}

void MySQLDataBase::init()
{
	mUser = "root";
	mPassword = "zhourui";
	mHost = "127.0.0.1";
	//mHost = ServerConfig::getStringParam(SDS_DOMAIN_NAME);
	mPort = 3306;
	mysql_init(mMySQL);
	connectDataBase(DATABASE_MAHJONG_USERDATA);
	mSQLAccount->init(mMySQL);
	mSQLCharacterData->init(mMySQL);
}

bool MySQLDataBase::connectDataBase(const std::string& dataBase)
{
	// 如果当前已经连接了这个数据库,则不再连接
	if (mConnectedDataBase == dataBase)
	{
		return true;
	}
	if (!mysql_real_connect(mMySQL, mHost.c_str(), mUser.c_str(), mPassword.c_str(), dataBase.c_str(), mPort, NULL, 0))
	{
		const char* error = mysql_error(mMySQL);
		LOG_ERROR("can not connect DataBase %s!   %s", dataBase.c_str(), error);
		return false;
	}
	int ret = mysql_query(mMySQL, "SET NAMES UTF8"); //设置编码格式
	if (ret != 0)
	{
		LOG_ERROR("set names error!   %s", mysql_error(mMySQL));
		return false;
	}
	mConnectedDataBase = dataBase;
	return true;
}

void MySQLDataBase::destroy()
{
	TRACE_DELETE(mSQLAccount);
	TRACE_DELETE(mSQLCharacterData);
	// 断开连接
	mysql_close(mMySQL);
	TRACE_DELETE(mMySQL);
}

int MySQLDataBase::getMaxGUID()
{
	return mSQLAccount->getMaxGUID();
}

bool MySQLDataBase::queryLogin(const std::string& account, const std::string& password, CHAR_GUID& guid, bool robotLogin)
{
	AccountTable* tableData = TRACE_NEW(AccountTable, tableData);
	bool ret = mSQLAccount->queryData(tableData, account, password);
	if (ret)
	{
		// 在不是进行机器人登录操作时登录了机器人账号,则返回失败
		if (!robotLogin && tableData->mIsRobot)
		{
			ret = false;
		}
		guid = tableData->mGUID;
	}
	TRACE_DELETE(tableData);
	return ret;
}

void MySQLDataBase::notifyAccountLogin(CHAR_GUID guid, bool login)
{
	mSQLAccount->notifyAccountLogin(guid, login);
}

bool MySQLDataBase::queryCharacterData(CHAR_GUID guid, std::string& name, int& money, short& head)
{
	CharacterDataTable* tableData = TRACE_NEW(CharacterDataTable, tableData);
	bool ret = mSQLCharacterData->queryCharacterData(tableData, guid);
	if (ret)
	{
		name = tableData->mName;
		money = tableData->mMoney;
		head = tableData->mHead;
	}
	TRACE_DELETE(tableData);
	return ret;
}

bool MySQLDataBase::isAccountExist(const std::string& account)
{
	return mSQLAccount->isAccountExist(account);
}

bool MySQLDataBase::isNameExist(const std::string& name)
{
	return mSQLCharacterData->isNameExist(name);
}

bool MySQLDataBase::getFirstNotLoginAccount(std::string& account, std::string& password)
{
	return mSQLAccount->getFirstNotLoginRobot(account, password);
}

int MySQLDataBase::registerAccount(const std::string& account, const std::string& password, const std::string& name, int money, int head, bool isRobot)
{
	if (isAccountExist(account))
	{
		return -1;
	}
	if (isNameExist(name))
	{
		return -2;
	}
	int curMaxGUID = getMaxGUID();
	if (curMaxGUID < 0)
	{
		return -3;
	}
	// 插入账号信息
	if (!mSQLAccount->registerAccount(account, password, ++curMaxGUID, isRobot))
	{
		return -4;
	}

	// 向角色数据表中插入数据
	if (!mSQLCharacterData->registeAccount(money, head, name, curMaxGUID))
	{
		return -5;
	}
	return 0;
}
