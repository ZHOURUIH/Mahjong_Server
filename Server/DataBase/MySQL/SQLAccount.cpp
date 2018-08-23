#include "SQLAccount.h"
#include "Utility.h"
#include "ServerConfig.h"
#include "MySQLDataBase.h"

const char* SQLAccount::COL_ACCOUNT = "account";
const char* SQLAccount::COL_PASSWORD = "password";
const char* SQLAccount::COL_IS_ROBOT = "isRobot";
const char* SQLAccount::COL_IS_LOGIN = "isLogin";
const char* SQLAccount::COL_GUID = "guid";

bool SQLAccount::queryData(AccountTable* tableData, const std::string& account, const std::string& password)
{
	char queryStr[256];
	SPRINTF(queryStr, 256, "SELECT * FROM %s WHERE %s = %s && %s = %s", mTableName, COL_ACCOUNT, ("\"" + account + "\"").c_str(), COL_PASSWORD, ("\"" + password + "\"").c_str());
	// 查询
	int ret = mysql_query(mMySQL, queryStr);
	if (ret != 0)
	{
		LOG_ERROR("query login error!   %s", mysql_error(mMySQL));
		return false;
	}
	tableData->mAccount = account;
	tableData->mPassword = password;
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
			tableData->mGUID = StringUtility::stringToInt(getColumn(rowData, COL_GUID));
			tableData->mIsRobot = StringUtility::stringToInt(getColumn(rowData, COL_IS_ROBOT)) > 0;
			tableData->mIsLogin = StringUtility::stringToInt(getColumn(rowData, COL_IS_LOGIN)) > 0;
			ret = true;
		}
		// 释放结果资源
		mysql_free_result(result);
		return ret;
	}
	return false;
}

void SQLAccount::notifyAccountLogin(CHAR_GUID guid, bool login)
{
	std::string guidStr = StringUtility::intToString(guid);
	char queryStr[256];
	SPRINTF(queryStr, 256, "UPDATE %s SET %s = %s WHERE %s = %s", mTableName, COL_IS_LOGIN, (login ? "1" : "0"), COL_GUID, guidStr.c_str());
	int ret = mysql_query(mMySQL, queryStr);
	if (ret != 0)
	{
		LOG_ERROR("can not set login state, guid : %s", guidStr.c_str());
	}
}

int SQLAccount::getMaxGUID()
{
	// 首先查询表中的最大GUID,用来生成用户的GUID
	char queryStr[256];
	SPRINTF(queryStr, 256, "SELECT max(%s) FROM %s", COL_GUID, mTableName);
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

bool SQLAccount::isAccountExist(const std::string& account)
{
	char queryStr[256];
	SPRINTF(queryStr, 256, "SELECT * FROM %s WHERE %s = %s", mTableName, COL_ACCOUNT, ("\"" + account + "\"").c_str());
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

bool SQLAccount::registerAccount(const std::string& account, const std::string& password, int guid, bool isRobot)
{
	std::string guidStr = StringUtility::intToString(guid);
	std::string isRobotStr = StringUtility::intToString(isRobot ? 1 : 0);
	std::string isLoginStr = "0";
	char insertAccountBuffer[256];
	SPRINTF(insertAccountBuffer, 256, "INSERT INTO %s VALUES(%s, %s, %s, %s, %s)", 
		mTableName, ("\"" + account + "\"").c_str(), ("\"" + password + "\"").c_str(), guidStr.c_str(), isRobotStr.c_str(), isLoginStr.c_str());
	int ret = mysql_query(mMySQL, insertAccountBuffer);
	if (ret != 0)
	{
		LOG_ERROR("insert account error!   %s", mysql_error(mMySQL));
		return false;
	}
	return true;
}

bool SQLAccount::getFirstNotLoginRobot(std::string& account, std::string& password)
{
	char queryStr[256];
	SPRINTF(queryStr, 256, "SELECT * FROM %s WHERE %s = %s && %s = %s LIMIT 1", mTableName, COL_IS_LOGIN, "0", COL_IS_ROBOT, "1");
	// 查询
	mysql_query(mMySQL, queryStr);
	bool ret = false;
	// 获得查询结果
	MYSQL_RES* result = mysql_store_result(mMySQL);
	if (result != NULL)
	{
		if (result->row_count > 0)
		{
			txVector<txMap<std::string, std::string>> resultData;
			getResultData(resultData, result);
			account = getColumn(resultData[0], COL_ACCOUNT);
			password = getColumn(resultData[0], COL_PASSWORD);
			ret = true;
		}
		// 释放结果资源
		mysql_free_result(result);
	}
	return ret;
}