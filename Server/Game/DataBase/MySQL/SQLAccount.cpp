#include "SQLAccount.h"
#include "Utility.h"
#include "ServerConfig.h"
#include "MySQLDataBase.h"

const char* SQLAccount::COL_ACCOUNT = "account";
const char* SQLAccount::COL_PASSWORD = "password";
const char* SQLAccount::COL_IS_ROBOT = "isRobot";
const char* SQLAccount::COL_GUID = "guid";

bool SQLAccount::queryData(AccountTable* tableData)
{
	char queryStr[256];
	SPRINTF(queryStr, 256, "SELECT * FROM %s WHERE %s = %s && %s = %s", mTableName, COL_ACCOUNT, ("\"" + tableData->mAccount + "\"").c_str(), COL_PASSWORD, ("\"" + tableData->mPassword + "\"").c_str());
	// 查询
	int ret = mysql_query(mMySQL, queryStr);
	if (ret != 0)
	{
		LOG_ERROR("query login error!   " + std::string(mysql_error(mMySQL)));
		return false;
	}
	// 获得查询结果
	MYSQL_RES* result = mysql_store_result(mMySQL);
	if (result != NULL)
	{
		bool ret = false;
		if (result->row_count > 0)
		{
			txVector<txMap<std::string, std::string>> resultDataList;
			getResultData(resultDataList, result);
			tableData->mGUID = StringUtility::stringToInt(getColumn(resultDataList[0], COL_GUID));
			tableData->mIsRobot = StringUtility::stringToInt(getColumn(resultDataList[0], COL_IS_ROBOT)) > 0;
			ret = true;
		}
		// 释放结果资源
		mysql_free_result(result);
		return ret;
	}
	return false;
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
		LOG_ERROR("query max GUID error!   " + std::string(mysql_error(mMySQL)));
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

bool SQLAccount::registerAccount(AccountTable* tableData)
{
	std::string guidStr = StringUtility::intToString(tableData->mGUID);
	std::string isRobotStr = StringUtility::intToString(tableData->mIsRobot ? 1 : 0);
	char insertAccountBuffer[256];
	SPRINTF(insertAccountBuffer, 256, "INSERT INTO %s VALUES(%s, %s, %s, %s)", 
		mTableName, ("\"" + tableData->mAccount + "\"").c_str(), ("\"" + tableData->mPassword + "\"").c_str(), guidStr.c_str(), isRobotStr.c_str());
	int ret = mysql_query(mMySQL, insertAccountBuffer);
	if (ret != 0)
	{
		LOG_ERROR("insert account error!   " + std::string(mysql_error(mMySQL)));
		return false;
	}
	return true;
}

bool SQLAccount::getAllRobotAccount(txMap<CHAR_GUID, AccountTable*>& robotAccountList)
{
	char queryStr[256];
	SPRINTF(queryStr, 256, "SELECT * FROM %s WHERE %s = %s", mTableName, COL_IS_ROBOT, "1");
	// 查询
	mysql_query(mMySQL, queryStr);
	bool ret = false;
	// 获得查询结果
	MYSQL_RES* result = mysql_store_result(mMySQL);
	if (result != NULL)
	{
		if (result->row_count > 0)
		{
			txVector<txMap<std::string, std::string>> resultDataList;
			getResultData(resultDataList, result);
			int count = resultDataList.size();
			FOR(resultDataList, int i = 0; i < count; ++i)
			{
				AccountTable* accountData = TRACE_NEW(AccountTable, accountData);
				accountData->mAccount = getColumn(resultDataList[i], COL_ACCOUNT);
				accountData->mPassword = getColumn(resultDataList[i], COL_PASSWORD);
				accountData->mGUID = StringUtility::stringToInt(getColumn(resultDataList[i], COL_GUID));
				accountData->mIsRobot = StringUtility::stringToInt(getColumn(resultDataList[i], COL_IS_ROBOT)) > 0;
				robotAccountList.insert(accountData->mGUID, accountData);
			}
			END(resultDataList);
			ret = true;
		}
		// 释放结果资源
		mysql_free_result(result);
	}
	return ret;
}