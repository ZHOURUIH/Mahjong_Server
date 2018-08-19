#include "MySQLDataBase.h"
#include "GameLog.h"
#include "Utility.h"
#include "ServerConfig.h"

const char* MySQLDataBase::DATABASE_MAHJONG_USERDATA = "mahjong_userdata";
const char* MySQLDataBase::TABLE_ACCOUNT = "account";
const char* MySQLDataBase::TABLE_CHARACTER_DATA = "character_data";
const char* MySQLDataBase::COL_ACCOUNT = "account";
const char* MySQLDataBase::COL_PASSWORD = "password";
const char* MySQLDataBase::COL_GUID = "guid";
const char* MySQLDataBase::COL_NAME = "name";
const char* MySQLDataBase::COL_MONEY = "money";
const char* MySQLDataBase::COL_HEAD = "head";

void MySQLDataBase::init()
{
	mUser = "root";
	mPassword = "zhourui";
	mHost = "127.0.0.1";
	//mHost = ServerConfig::getStringParam(SDS_DOMAIN_NAME);
	mPort = 3306;
	mMySQL = TRACE_NEW(MYSQL, mMySQL);
	mysql_init(mMySQL);
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
	// 断开连接
	mysql_close(mMySQL);
	TRACE_DELETE(mMySQL);
}

int MySQLDataBase::getMaxGUID()
{
	if (!connectDataBase(DATABASE_MAHJONG_USERDATA))
	{
		return -1;
	}

	// 首先查询表中的最大GUID,用来生成用户的GUID
	char queryStr[256];
	SPRINTF(queryStr, 256, "select max(%s) from %s", COL_GUID, TABLE_ACCOUNT);
	// 查询
	int ret = mysql_query(mMySQL, queryStr);
	if (ret != 0)
	{
		LOG_ERROR("query max GUID error!   %s", mysql_error(mMySQL));
		return -1;
	}
	// 获得查询结果
	MYSQL_RES* result = mysql_store_result(mMySQL);
	if (result == NULL)
	{
		return -1;
	}
	int guid = 0;
	if (result->row_count > 0)
	{
		MYSQL_ROW sql_row = mysql_fetch_row(result);
		if (sql_row != NULL && sql_row[0] != NULL)
		{
			guid = StringUtility::stringToInt(sql_row[0]);
		}
	}
	// 释放查询结果
	mysql_free_result(result);
	return guid;
}

bool MySQLDataBase::queryLogin(const std::string& account, const std::string& password, CHAR_GUID& guid)
{
	if (!connectDataBase(DATABASE_MAHJONG_USERDATA))
	{
		return false;
	}
	char queryStr[256];
	SPRINTF(queryStr, 256, "select * from %s where %s = %s && %s = %s", TABLE_ACCOUNT, COL_ACCOUNT, ("\"" + account + "\"").c_str(), COL_PASSWORD, ("\"" + password + "\"").c_str());
	// 查询
	int ret = mysql_query(mMySQL, queryStr);
	if (ret != 0)
	{
		LOG_ERROR("query login error!   %s", mysql_error(mMySQL));
		return false;
	}
	// 获得查询结果
	MYSQL_RES* result = mysql_store_result(mMySQL);
	if (result != NULL)
	{
		bool ret = false;
		if (result->row_count > 0)
		{
			txMap<std::string, std::string> rowData;
			MYSQL_ROW sql_row = mysql_fetch_row(result);
			int fieldCount = mysql_num_fields(result);
			for (int i = 0; i < fieldCount; ++i)
			{
				MYSQL_FIELD* fd = mysql_fetch_field(result);
				rowData.insert(fd->name, sql_row[i]);
			}
			guid = StringUtility::stringToInt(getColumn(rowData, COL_GUID, TABLE_ACCOUNT));
			ret = true;
		}
		// 释放结果资源
		mysql_free_result(result);
		return ret;
	}
	return false;
}

bool MySQLDataBase::queryCharacterData(CHAR_GUID guid, std::string& name, int& money, short& head)
{
	if (!connectDataBase(DATABASE_MAHJONG_USERDATA))
	{
		return false;
	}
	std::string guidStr = StringUtility::intToString(guid);
	char queryStr[256];
	SPRINTF(queryStr, 256, "select * from %s where %s = %s", TABLE_CHARACTER_DATA, COL_GUID, guidStr.c_str());
	// 查询
	int ret = mysql_query(mMySQL, queryStr);
	if (ret != 0)
	{
		LOG_ERROR("query character data error!   %s", mysql_error(mMySQL));
		return false;
	}
	// 获得查询结果
	MYSQL_RES* result = mysql_store_result(mMySQL);
	if (result != NULL)
	{
		bool ret = false;
		if (result->row_count > 0)
		{
			txMap<std::string, std::string> rowData;
			MYSQL_ROW sql_row = mysql_fetch_row(result);
			int fieldCount = mysql_num_fields(result);
			for (int i = 0; i < fieldCount; ++i)
			{
				MYSQL_FIELD* fd = mysql_fetch_field(result);
				rowData.insert(fd->name, sql_row[i]);
			}
			name = getColumn(rowData, COL_NAME, TABLE_CHARACTER_DATA);
			money = StringUtility::stringToInt(getColumn(rowData, COL_MONEY, TABLE_CHARACTER_DATA));
			head = StringUtility::stringToInt(getColumn(rowData, COL_HEAD, TABLE_CHARACTER_DATA));
			ret = true;
		}
		// 释放结果资源
		mysql_free_result(result);
		return ret;
	}
	return false;
}

const std::string& MySQLDataBase::getColumn(txMap<std::string, std::string>& rowData, const std::string& col, const std::string& table)
{
	if (rowData.find(col) == rowData.end())
	{
		LOG_ERROR("can not find %s in table %s", col.c_str(), table.c_str());
		return EMPTY_STRING;
	}
	else
	{
		return rowData[col];
	}
}

bool MySQLDataBase::isAccountExist(const std::string& account)
{
	if (!connectDataBase(DATABASE_MAHJONG_USERDATA))
	{
		return false;
	}
	char queryStr[256];
	SPRINTF(queryStr, 256, "select * from %s where %s = %s", TABLE_ACCOUNT, COL_ACCOUNT, ("\"" + account + "\"").c_str());
	// 查询
	mysql_query(mMySQL, queryStr);
	// 获得查询结果
	MYSQL_RES* result = mysql_store_result(mMySQL);
	if (result != NULL)
	{
		// 如果有查询结果,则返回账号已存在
		bool ret = result->row_count > 0;
		// 释放结果资源
		mysql_free_result(result);
		return ret;
	}
	return false;
}

bool MySQLDataBase::isNameExist(const std::string& name)
{
	if (!connectDataBase(DATABASE_MAHJONG_USERDATA))
	{
		return false;
	}
	char queryStr[256];
	SPRINTF(queryStr, 256, "select * from %s where %s = %s", TABLE_CHARACTER_DATA, COL_NAME, ("\"" + name + "\"").c_str());
	// 查询
	mysql_query(mMySQL, queryStr);
	// 获得查询结果
	MYSQL_RES* result = mysql_store_result(mMySQL);
	if (result != NULL)
	{
		// 如果有查询结果,则返回账号已存在
		bool ret = result->row_count > 0;
		// 释放结果资源
		mysql_free_result(result);
		return ret;
	}
	return false;
}

int MySQLDataBase::registerAccount(const std::string& account, const std::string& password, const std::string& name, int money, int head)
{
	if (!connectDataBase(DATABASE_MAHJONG_USERDATA))
	{
		return -3;
	}
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
	std::string guidStr = StringUtility::intToString(++curMaxGUID);
	char insertAccountBuffer[256];
	SPRINTF(insertAccountBuffer, 256, "insert into %s values(%s, %s, %s)", TABLE_ACCOUNT, ("\"" + account + "\"").c_str(), ("\"" + password + "\"").c_str(), guidStr.c_str());
	int ret = mysql_query(mMySQL, insertAccountBuffer);
	if (ret != 0)
	{
		LOG_ERROR("insert account error!   %s", mysql_error(mMySQL));
		return -4;
	}

	// 向角色数据表中插入数据
	std::string moneyStr = StringUtility::intToString(money);
	std::string headStr = StringUtility::intToString(head);
	char insertCharacterDataBuffer[256];
	SPRINTF(insertCharacterDataBuffer, 256, "insert into %s values(%s, %s, %s, %s)", TABLE_CHARACTER_DATA, guidStr.c_str(), ("\"" + name + "\"").c_str(), moneyStr.c_str(), headStr.c_str());
	ret = mysql_query(mMySQL, insertCharacterDataBuffer);
	if (ret != 0)
	{
		LOG_ERROR("insert character data error!   %s", mysql_error(mMySQL));
		return -5;
	}
	return 0;
}
