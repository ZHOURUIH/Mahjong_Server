#include "MySQLDataBase.h"
#include "GameLog.h"

std::string MySQLDataBase::mDataBase = "test";
std::string MySQLDataBase::mAccountTable = "Account";
std::string MySQLDataBase::mCharacterDataTable = "CharacterData";
std::string MySQLDataBase::mAccountColumnName = "account";
std::string MySQLDataBase::mPasswordColumnName = "password";
std::string MySQLDataBase::mGUIDColumnName = "guid";
std::string MySQLDataBase::mNameColumnName = "name";
std::string MySQLDataBase::mModelColumnName = "model";

void MySQLDataBase::init(const std::string& user, const std::string& pswd, const std::string& host, const int& port)
{
	mUser = user;
	mPassword = pswd;
	mHost = host;
	mPort = port;
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
		GAME_ERROR("can not connect DataBase %s!", dataBase.c_str());
		return false;
	}
	mysql_query(mMySQL, "SET NAMES GBK"); //设置编码格式
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
	if (!connectDataBase(mDataBase))
	{
		return -1;
	}

	// 首先查询表中的最大GUID,用来生成用户的GUID
	std::string queryStr = "select max(" + mGUIDColumnName + ") from " + mAccountTable;
	// 查询
	mysql_query(mMySQL, queryStr.c_str());
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
			guid = txUtility::stringToInt(sql_row[0]);
		}
	}
	// 释放查询结果
	mysql_free_result(result);
	return guid;
}

bool MySQLDataBase::queryLogin(const std::string& account, const std::string& password, std::string& playerName, CHAR_GUID& guid)
{
	if (!connectDataBase(mDataBase))
	{
		return false;
	}
	std::string queryStr = "select * from " + mAccountTable + " where " + mAccountColumnName + " = \"" + account + "\" && " + mPasswordColumnName + " = \"" + password + "\"";
	// 查询
	mysql_query(mMySQL, queryStr.c_str());
	// 获得查询结果
	MYSQL_RES* result = mysql_store_result(mMySQL);
	if (result != NULL)
	{
		bool ret = false;
		if (result->row_count > 0)
		{
			std::map<std::string, std::string> rowData;
			MYSQL_ROW sql_row = mysql_fetch_row(result);
			int fieldCount = mysql_num_fields(result);
			for (int i = 0; i < fieldCount; ++i)
			{
				MYSQL_FIELD* fd = mysql_fetch_field(result);
				rowData.insert(std::make_pair(fd->name, sql_row[i]));
			}
			playerName = rowData[mNameColumnName];
			guid = txUtility::stringToInt(rowData[mGUIDColumnName]);
			ret = true;
		}
		// 释放结果资源
		mysql_free_result(result);
		return ret;
	}
	return false;
}

bool MySQLDataBase::isAccountExist(const std::string& account)
{
	if (!connectDataBase(mDataBase))
	{
		return false;
	}
	std::string queryStr = "select * from " + mAccountTable + " where " + mAccountColumnName + " = \"" + account + "\"";
	// 查询
	mysql_query(mMySQL, queryStr.c_str());
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
	if (!connectDataBase(mDataBase))
	{
		return false;
	}
	std::string queryStr = "select * from " + mCharacterDataTable + " where " + mNameColumnName + " = \"" + name + "\"";
	// 查询
	mysql_query(mMySQL, queryStr.c_str());
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

int MySQLDataBase::registerAccount(const std::string& account, const std::string& password, const std::string& name, const int& money, const int& head)
{
	if (!connectDataBase(mDataBase))
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
	std::string guidStr = txUtility::intToString(++curMaxGUID);
	std::string insertAccountStr = "insert into " + mAccountTable + " values(" + guidStr + ", \"" + account + "\", " + "\"" + password;
	mysql_query(mMySQL, insertAccountStr.c_str());

	// 向角色数据表中插入数据
	std::string moneyStr = txUtility::intToString(money);
	std::string headStr = txUtility::intToString(head);
	std::string insertCharacterDataStr = "insert into " + mCharacterDataTable + " values(" + guidStr + ", \"" + name + "\", " + moneyStr + ", " + headStr;
	mysql_query(mMySQL, insertCharacterDataStr.c_str());
	return 0;
}
