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
		LOG_ERROR("can not connect DataBase " + dataBase + "!   " + std::string(error));
		return false;
	}
	int ret = mysql_query(mMySQL, "SET NAMES UTF8"); //设置编码格式
	if (ret != 0)
	{
		LOG_ERROR("set names error!   " + std::string(mysql_error(mMySQL)));
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

bool MySQLDataBase::queryLogin(AccountTable* data)
{
	bool isRobot = data->mIsRobot;
	bool ret = mSQLAccount->queryData(data);
	if (ret)
	{
		// 在不是进行机器人登录操作时登录了机器人账号,则返回失败
		if (!isRobot && data->mIsRobot)
		{
			ret = false;
		}
	}
	return ret;
}

bool MySQLDataBase::queryCharacterData(CHAR_GUID guid, CharacterDataTable* data)
{
	return mSQLCharacterData->queryCharacterData(guid, data);
}

bool MySQLDataBase::isAccountExist(const std::string& account)
{
	return mSQLAccount->isAccountExist(account);
}

bool MySQLDataBase::isNameExist(const std::string& name)
{
	return mSQLCharacterData->isNameExist(name);
}

bool MySQLDataBase::getAllRobotAccount(txMap<CHAR_GUID, AccountTable*>& robotAccountList)
{
	return mSQLAccount->getAllRobotAccount(robotAccountList);
}

int MySQLDataBase::registerAccount(AccountTable* accountData, CharacterDataTable* data)
{
	if (isAccountExist(accountData->mAccount))
	{
		return -1;
	}
	if (isNameExist(data->mName))
	{
		return -2;
	}
	int curMaxGUID = getMaxGUID();
	if (curMaxGUID < 0)
	{
		return -3;
	}
	accountData->mGUID = ++curMaxGUID;
	data->mGUID = accountData->mGUID;
	// 插入账号信息
	if (!mSQLAccount->registerAccount(accountData))
	{
		return -4;
	}
	// 向角色数据表中插入数据
	if (!mSQLCharacterData->registeAccount(data))
	{
		return -5;
	}
	return 0;
}
