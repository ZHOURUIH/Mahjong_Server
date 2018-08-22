#include "SQLCharacterData.h"
#include "Utility.h"

const char* SQLCharacterData::COL_GUID = "guid";
const char* SQLCharacterData::COL_NAME = "name";
const char* SQLCharacterData::COL_MONEY = "money";
const char* SQLCharacterData::COL_HEAD = "head";

bool SQLCharacterData::queryCharacterData(CharacterDataTable* tableData, CHAR_GUID guid)
{
	std::string guidStr = StringUtility::intToString(guid);
	char queryStr[256];
	SPRINTF(queryStr, 256, "SELECT * FROM %s WHERE %s = %s", mTableName, COL_GUID, guidStr.c_str());
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
			if (result->row_count != 1)
			{
				LOG_ERROR("find more than 1 row character data! guid : %d", guid);
			}
			txVector<txMap<std::string, std::string>> resultData;
			getResultData(resultData, result);
			// 字符串类型的字段需要转换为GB2312编码
			tableData->mName = StringUtility::UTF8ToANSI(getColumn(resultData[0], COL_NAME));
			tableData->mMoney = StringUtility::stringToInt(getColumn(resultData[0], COL_MONEY));
			tableData->mHead = StringUtility::stringToInt(getColumn(resultData[0], COL_HEAD));
			ret = true;
		}
		// 释放结果资源
		mysql_free_result(result);
		return ret;
	}
	return false;
}

bool SQLCharacterData::isNameExist(const std::string& name)
{
	char queryStr[256];
	SPRINTF(queryStr, 256, "SELECT * FROM %s WHERE %s = %s", mTableName, COL_NAME, ("\"" + name + "\"").c_str());
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

bool SQLCharacterData::registeAccount(int money, int head, const std::string& name, int guid)
{
	std::string moneyStr = StringUtility::intToString(money);
	std::string headStr = StringUtility::intToString(head);
	std::string guidStr = StringUtility::intToString(guid);
	char insertCharacterDataBuffer[256];
	SPRINTF(insertCharacterDataBuffer, 256, "INSERT INTO %s VALUES(%s, %s, %s, %s)", mTableName, guidStr.c_str(), ("\"" + name + "\"").c_str(), moneyStr.c_str(), headStr.c_str());
	int ret = mysql_query(mMySQL, insertCharacterDataBuffer);
	if (ret != 0)
	{
		LOG_ERROR("insert character data error!   %s", mysql_error(mMySQL));
		return false;
	}
	return true;
}